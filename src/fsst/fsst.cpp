// This file is based on the file fsst.cpp from the FSST library
// (https://github.com/cwida/fsst) Which is provided under MIT License
// (http://www.opensource.org/licenses/MIT) Copyright (c) 2018-2020, CWI, TU
// Munich, FSU Jena
//
// Changes to the original code are marked with the comment "MODIFIED:
// [Description of the Modification]" to make them easy to find.

// MODIFIED: Modified the location of the original fsst.h file and added a
// second, new fsst.h file
#include "../../lib/fsst/fsst.h"
#include "fsst.h"
// END OF MODIFIED

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <iomanip>
// MODIFIED: Addition of the chrono library
#include <chrono>
// END OF MODIFIED
using namespace std;

// Utility to compress and decompress (-d) data with FSST (using stdin and
// stdout).
//
// The utility has a poor-man's async I/O in that it uses double buffering for
// input and output, and two background pthreads for reading and writing. The
// idea is to make the CPU overlap with I/O.
//
// The data format is quite simple. A FSST compressed file is a sequence of
// blocks, each with format: (1) 3-byte block length field (max blocksize is
// hence 16MB). This byte-length includes (1), (2) and (3). (2) FSST dictionary
// as produced by fst_export(). (3) the FSST compressed data.
//
// The natural strength of FSST is in fact not block-based compression, but
// rather the compression and *individual* decompression of many small strings
// separately. Think of compressed databases and (column-store) data formats.
// But, this utility is to serve as an apples-to-apples comparison point with
// utilities like lz4.

namespace
{

   class BinarySemaphore
   {
   private:
      mutex m;
      condition_variable cv;
      bool value;

   public:
      explicit BinarySemaphore(bool initialValue = false) : value(initialValue) {}
      void wait()
      {
         unique_lock<mutex> lock(m);
         while (!value)
            cv.wait(lock);
         value = false;
      }
      void post()
      {
         {
            unique_lock<mutex> lock(m);
            value = true;
         }
         cv.notify_one();
      }
   };

   bool stopThreads = false;
   BinarySemaphore srcDoneIO[2], dstDoneIO[2], srcDoneCPU[2], dstDoneCPU[2];
   unsigned char *srcBuf[2] = {NULL, NULL};
   unsigned char *dstBuf[2] = {NULL, NULL};
   unsigned char *dstMem[2] = {NULL, NULL};
   size_t srcLen[2] = {0, 0};
   size_t dstLen[2] = {0, 0};

#define FSST_MEMBUF (1ULL << 22)
   int decompress = 0;
   size_t blksz =
       FSST_MEMBUF -
       (1 +
        FSST_MAXHEADER /
            2); // block size of compression (max compressed size must fit 3 bytes)

#define DESERIALIZE(p)                                                          \
   (((unsigned long long)(p)[0]) << 16) | (((unsigned long long)(p)[1]) << 8) | \
       ((unsigned long long)(p)[2])
#define SERIALIZE(l, p)           \
   {                              \
      (p)[0] = ((l) >> 16) & 255; \
      (p)[1] = ((l) >> 8) & 255;  \
      (p)[2] = (l) & 255;         \
   }

   void reader(ifstream &src)
   {
      for (int swap = 0; true; swap = 1 - swap)
      {
         srcDoneCPU[swap].wait();
         if (stopThreads)
            break;
         src.read((char *)srcBuf[swap], blksz);
         srcLen[swap] = (unsigned long)src.gcount();
         if (decompress)
         {
            if (blksz && srcLen[swap] == blksz)
            {
               blksz =
                   DESERIALIZE(srcBuf[swap] + blksz - 3); // read size of next block
               srcLen[swap] -= 3;                         // cut off size bytes
            }
            else
            {
               blksz = 0;
            }
         }
         srcDoneIO[swap].post();
      }
   }

   void writer(ofstream &dst)
   {
      for (int swap = 0; true; swap = 1 - swap)
      {
         dstDoneCPU[swap].wait();
         if (!dstLen[swap])
            break;
         dst.write((char *)dstBuf[swap], dstLen[swap]);
         dstDoneIO[swap].post();
      }
      for (int swap = 0; swap < 2; swap++)
         dstDoneIO[swap].post();
   }

} // namespace

int main(int argc, char *argv[])
{
   // MODIFIED: Added the timer for measuring the compression time
   auto start = std::chrono::high_resolution_clock::now();
   auto symbol_table_start = std::chrono::high_resolution_clock::now();
   auto symbol_table_end = std::chrono::high_resolution_clock::now();
   auto compression_start = std::chrono::high_resolution_clock::now();
   auto compression_end = std::chrono::high_resolution_clock::now();
   // END OF MODIFIED

   // MODIFIED: Added an additional input to define the number lines to sample
   size_t srcTot = 0, dstTot = 0;

   if (argc < 2 || argc > 5 ||
       (argc == 5 && (argv[1][0] != '-' || argv[1][1] != 'd' || argv[1][2])) ||
       (argc == 4 && (argv[1][0] == '-' && argv[1][1] == 'd')))
   {
      cerr << "usage: " << argv[0] << " -d infile outfile" << endl;
      cerr << "       " << argv[0] << " ['fsst' for fsst_sampling or '[pos. int]' for number of lines to sample] infile" << endl;
      cerr << "       " << argv[0] << " ['fsst' for fsst_sampling or '[pos. int]' for number of lines to sample] infile outfile" << endl;
      return -1;
   }

   bool decompress = (argc == 5);
   bool fsst_sampling = false;
   int argOffset = 1;
   int number_lines_to_sample = 0;

   if (!decompress)
   {
      string firstArg = argv[1];
      if (firstArg == "fsst")
      {
         fsst_sampling = true;
         argOffset = 2;
      }
      else
      {
         try
         {
            number_lines_to_sample = stoi(firstArg); // Tries to convert the first argument to an integer
            if (number_lines_to_sample <= 0)
            {
               throw std::invalid_argument("Number must be positive");
            }
            argOffset = 2;
         }
         catch (const std::invalid_argument &)
         {
            cerr << "Invalid argument. Must be a positive integer or 'fsst'." << endl;
            return -1;
         }
      }
   }

   string srcfile(argv[argOffset]), dstfile;
   if (argc == argOffset + 1) // Only infile given
   {
      dstfile = srcfile + ".fsst";
   }
   else
   {
      dstfile = argv[argOffset + 1];
   }
   // END OF MODIFIED

   ifstream src;
   ofstream dst;
   src.open(srcfile, ios::binary);
   dst.open(dstfile, ios::binary);
   dst.exceptions(ios_base::failbit);
   dst.exceptions(ios_base::badbit);
   src.exceptions(ios_base::badbit);
   if (decompress)
   {
      unsigned char tmp[3];
      src.read((char *)tmp, 3);
      if (src.gcount() != 3)
      {
         cerr << "failed to open input." << endl;
         return -1;
      }
      blksz = DESERIALIZE(tmp); // read first block size
   }
   vector<unsigned char> buffer(FSST_MEMBUF * 6);
   srcBuf[0] = buffer.data();
   srcBuf[1] = srcBuf[0] + (FSST_MEMBUF * (1ULL + decompress));
   dstMem[0] = srcBuf[1] + (FSST_MEMBUF * (1ULL + decompress));
   dstMem[1] = dstMem[0] + (FSST_MEMBUF * (2ULL - decompress));

   for (int swap = 0; swap < 2; swap++)
   {
      srcDoneCPU[swap].post(); // input buffer is not being processed initially
      dstDoneIO[swap].post();  // output buffer is not being written initially
   }
   thread readerThread([&src]
                       { reader(src); });
   thread writerThread([&dst]
                       { writer(dst); });

   for (int swap = 0; true; swap = 1 - swap)
   {
      srcDoneIO[swap]
          .wait(); // wait until input buffer is available (i.e. done reading)
      dstDoneIO[swap]
          .wait(); // wait until output buffer is ready writing hence free for use
      if (srcLen[swap] == 0)
      {
         dstLen[swap] = 0;
         break;
      }
      if (decompress)
      {
         fsst_decoder_t decoder;
         size_t hdr = fsst_import(&decoder, srcBuf[swap]);
         dstLen[swap] =
             fsst_decompress(&decoder, srcLen[swap] - hdr, srcBuf[swap] + hdr,
                             FSST_MEMBUF, dstBuf[swap] = dstMem[swap]);
      }
      else
      {
         unsigned char tmp[FSST_MAXHEADER];

         // MODIFIED: Use of our function fsst_create_with_fst instead of the
         fsst_encoder_t *encoder;

         symbol_table_start = std::chrono::high_resolution_clock::now();
         if (!fsst_sampling)
         {
            encoder = fsst_create_with_fst(number_lines_to_sample, &srcBuf[swap]);
         }
         else
         {
            encoder = fsst_create_with_fst_w_fsst_sampling(1, &srcLen[swap], &srcBuf[swap], 0);
         }
         symbol_table_end = std::chrono::high_resolution_clock::now();
         // END OF MODIFIED

         // MODIFIED: Added a timer for measuring the compression time
         compression_start = std::chrono::high_resolution_clock::now();
         // END OF MODIFIED
         size_t hdr = fsst_export(encoder, tmp);
         if (fsst_compress(encoder, 1, &srcLen[swap], &srcBuf[swap],
                           FSST_MEMBUF * 2, dstMem[swap] + FSST_MAXHEADER + 3,
                           &dstLen[swap], &dstBuf[swap]) < 1)
            return -1;
         // MODIFIED: Added a timer for measuring the compression time
         compression_end = std::chrono::high_resolution_clock::now();
         // END OF MODIFIED
         dstLen[swap] += 3 + hdr;
         dstBuf[swap] -= 3 + hdr;
         SERIALIZE(dstLen[swap], dstBuf[swap]); // block starts with size
         copy(tmp, tmp + hdr,
              dstBuf[swap] + 3); // then the header (followed by the compressed
                                 // bytes which are already there)
         fsst_destroy(encoder);
      }
      srcTot += srcLen[swap];
      dstTot += dstLen[swap];
      srcDoneCPU[swap]
          .post();             // input buffer may be re-used by the reader for the next block
      dstDoneCPU[swap].post(); // output buffer is ready for writing out
   }
   cerr << (decompress ? "Dec" : "C") << "ompressed " << srcTot << " bytes into "
        << dstTot << " bytes ==> " << (int)((100 * dstTot) / srcTot) << "%"
        << endl;

   // force wait until all background writes finished
   stopThreads = true;
   for (int swap = 0; swap < 2; swap++)
   {
      srcDoneCPU[swap].post();
      dstDoneCPU[swap].post();
   }
   dstDoneIO[0].wait();
   dstDoneIO[1].wait();
   readerThread.join();
   writerThread.join();

   // MODIFIED: Added the timer for measuring the compression time
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> diff = end - start;
   std::cout << std::fixed << std::setprecision(6);
   std::cout << "Time to compress: " << diff.count() << " s\n";
   std::chrono::duration<double> symbol_table_diff = symbol_table_end - symbol_table_start;
   std::cout << "Time to create symbol table: " << symbol_table_diff.count() << " s\n";
   std::chrono::duration<double> compression_diff = compression_end - compression_start;
   std::cout << "Time to compress (without symbol table creation): " << compression_diff.count() << " s\n";
   std::cout << "Start of compression: " << std::chrono::duration_cast<std::chrono::milliseconds>(compression_start.time_since_epoch()).count() << " ms\n";
   std::cout << "End of compression: " << std::chrono::duration_cast<std::chrono::milliseconds>(compression_end.time_since_epoch()).count() << " ms\n";
   // END OF MODIFIED
}

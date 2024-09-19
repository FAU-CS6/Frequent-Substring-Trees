###########
# Imports #
###########

import subprocess
import os
import time
import csv
import re

############
# Settings #
############

# Path to the fsst_with_fst executable
fsst_with_fst_path = os.path.join(
    ".", "fsst_with_fst"
)  # This path expects an execution from the root directory of the project

# Path to the fsst_without_fst executable
fsst_without_fst_path = os.path.join(
    ".", "fsst_without_fst"
)  # This path expects an execution from the root directory of the project

# Path to the files to compress (directory)
benchmark_dir = os.path.join(
    "lib", "fsst", "paper", "dbtext"
)  # This path expects an execution from the root directory of the project

# Path to the output directory
output_dir = os.path.join(
    "benchmark", "running"
)  # This path expects an execution from the root directory of the project


####################
# Helper functions #
####################


def run_fsst_with_fst(input_file, number_lines_to_sample, output_file):
    command = [
        fsst_with_fst_path,
        str(number_lines_to_sample),
        input_file,
        output_file,
    ]
    return subprocess.check_output(command, stderr=subprocess.STDOUT).decode("utf-8")


def run_fsst_without_fst(input_file, output_file):
    command = [fsst_without_fst_path, input_file, output_file]
    return subprocess.check_output(command, stderr=subprocess.STDOUT).decode("utf-8")


def run_fsst_with_fst_and_fsst_sampling(input_file, output_file):
    command = [fsst_with_fst_path, "fsst", input_file, output_file]
    return subprocess.check_output(command, stderr=subprocess.STDOUT).decode("utf-8")


def extract_sizes(output_str):
    pattern = r"Compressed (\d+) bytes into (\d+) bytes ==> (\d+)%"
    match = re.search(pattern, output_str)
    if match:
        return int(match.group(1)), int(match.group(2)), int(match.group(3))
    return -1, -1, -1


def extract_times(output_str):
    pattern_compress = r"Time to compress: ([\d.]+) s"
    pattern_symbol_table = r"Time to create symbol table: ([\d.]+) s"
    pattern_compress_no_symbol = (
        r"Time to compress \(without symbol table creation\): ([\d.]+) s"
    )
    pattern_start_compression = r"Start of compression: ([\d.]+) ms"
    pattern_end_compression = r"End of compression: ([\d.]+) ms"
    match_compress = re.search(pattern_compress, output_str)
    match_symbol_table = re.search(pattern_symbol_table, output_str)
    match_compress_no_symbol = re.search(pattern_compress_no_symbol, output_str)
    match_start_compression = re.search(pattern_start_compression, output_str)
    match_end_compression = re.search(pattern_end_compression, output_str)
    time_compress = float(match_compress.group(1)) if match_compress else -1
    time_symbol_table = float(match_symbol_table.group(1)) if match_symbol_table else -1
    time_compress_no_symbol = (
        float(match_compress_no_symbol.group(1)) if match_compress_no_symbol else -1
    )
    start_compression = (
        float(match_start_compression.group(1)) / 1000.0
        if match_start_compression
        else -1
    )
    end_compression = (
        float(match_end_compression.group(1)) / 1000.0 if match_end_compression else -1
    )
    if -1 in [
        time_compress,
        time_symbol_table,
        time_compress_no_symbol,
        start_compression,
        end_compression,
    ]:
        print("Error while extracting a time. Output string:")
        print(output_str)
        print("Time to compress:", time_compress)
        print("Time to create symbol table:", time_symbol_table)
        print(
            "Time to compress (without symbol table creation):", time_compress_no_symbol
        )
        print("Start of compression:", start_compression)
        print("End of compression:", end_compression)
    return (
        time_compress,
        time_symbol_table,
        time_compress_no_symbol,
        start_compression,
        end_compression,
    )


def get_number_of_lines(file_path):
    try:
        with open(file_path, "r") as f:
            return sum(1 for _ in f)
    except:
        return -1


def create_custom_steps(end):
    steps = list(range(1, 25))  # Between 1 and 25 every step
    steps += list(range(25, 100, 5))  # Between 25 and 100 every 5th step
    steps += list(range(100, 500, 50))  # Between 100 and 500 every 50th step
    steps += list(range(500, end + 1, 100))  # Between 500 and end every 100th step
    return steps


############
# Main run #
############

if __name__ == "__main__":
    # Create a directory for the benchmark outputs
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Set up CSV logging
    with open(os.path.join(output_dir, "results.csv"), "w", newline="") as csvfile:
        fieldnames = [
            "name",
            "input_file",
            "number_lines_to_sample",
            "start",
            "duration",
            "duration_symbol_table",
            "duration_compress_no_symbol",
            "start_compress_no_symbol",
            "end_compress_no_symbol",
            "input_size",
            "output_size",
            "compression_ratio",
        ]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        # Create a directory for the compressed files
        compressed_files_dir = os.path.join(output_dir, "compressed_files")
        if not os.path.exists(compressed_files_dir):
            os.makedirs(compressed_files_dir)

        # Run each benchmark 10 times
        for _ in range(10):

            # Create a directory for the compressed files of the current run
            benchmark_run_dir = os.path.join(compressed_files_dir, "run_" + str(_))
            if not os.path.exists(benchmark_run_dir):
                os.makedirs(benchmark_run_dir)

            # Iterate through files in the directory
            for file in os.listdir(benchmark_dir):
                if not os.path.isfile(os.path.join(benchmark_dir, file)):
                    continue

                # Set the input file
                input_file = os.path.join(benchmark_dir, file)

                # Create a directory for the compressed versions of the current file
                compressed_input_file_dir = os.path.join(benchmark_run_dir, file)
                if not os.path.exists(compressed_input_file_dir):
                    os.makedirs(compressed_input_file_dir)

                # fsst_with_fst runs
                total_lines = get_number_of_lines(input_file)

                # Create a directory for the compressed versions of the current file with fsst_with_fst
                compressed_with_fst_dir = os.path.join(
                    compressed_input_file_dir, "fsst_with_fst"
                )
                if not os.path.exists(compressed_with_fst_dir):
                    os.makedirs(compressed_with_fst_dir)

                # Run fsst_with_fst with different settings for the number_lines_to_sample (up to min(total_lines, 1000) lines)
                for number_lines_to_sample in create_custom_steps(
                    min(total_lines, 1000)
                ):
                    start_time = time.time()

                    # Print current file and number of lines to sample
                    print(
                        "Running",
                        file,
                        "with",
                        number_lines_to_sample,
                        "lines sampled (fsst_with_fst)",
                    )

                    try:
                        compressed_with_fst_file = os.path.join(
                            compressed_with_fst_dir,
                            str(number_lines_to_sample) + "_lines_sampled.fsst",
                        )
                        output = run_fsst_with_fst(
                            input_file, number_lines_to_sample, compressed_with_fst_file
                        )
                        end_time = time.time()
                        input_size, output_size, compression_ratio = extract_sizes(
                            output
                        )
                        (
                            time_compress,
                            time_symbol_table,
                            time_compress_no_symbol,
                            start_compression,
                            end_compression,
                        ) = extract_times(output)

                    except subprocess.CalledProcessError:
                        end_time = time.time()
                        input_size, output_size, compression_ratio = -1, -1, -1
                        (
                            time_compress,
                            time_symbol_table,
                            time_compress_no_symbol,
                            start_compression,
                            end_compression,
                        ) = (
                            -1,
                            -1,
                            -1,
                            -1,
                            -1,
                        )
                        print("Error while running fsst_with_fst")

                    writer.writerow(
                        {
                            "name": "fsst_with_fst",
                            "input_file": file,
                            "number_lines_to_sample": number_lines_to_sample,
                            "start": start_time,
                            "duration": time_compress * 1000,  # Convert to milliseconds
                            "duration_symbol_table": time_symbol_table
                            * 1000,  # Convert to milliseconds
                            "duration_compress_no_symbol": time_compress_no_symbol
                            * 1000,  # Convert to milliseconds
                            "start_compress_no_symbol": start_compression * 1000,
                            "end_compress_no_symbol": end_compression * 1000,
                            "input_size": input_size,
                            "output_size": output_size,
                            "compression_ratio": compression_ratio,
                        }
                    )
                    csvfile.flush()

                # fsst_without_fst run
                start_time = time.time()

                # Print current file
                print("Running", file, "without FST (fsst_without_fst)")

                try:
                    compressed_without_fst_dir = os.path.join(
                        compressed_input_file_dir, "fsst_without_fst.fsst"
                    )
                    output = run_fsst_without_fst(
                        input_file, compressed_without_fst_dir
                    )
                    end_time = time.time()
                    input_size, output_size, compression_ratio = extract_sizes(output)
                    (
                        time_compress,
                        time_symbol_table,
                        time_compress_no_symbol,
                        start_compression,
                        end_compression,
                    ) = extract_times(output)

                except subprocess.CalledProcessError:
                    end_time = time.time()
                    input_size, output_size, compression_ratio = -1, -1, -1
                    (
                        time_compress,
                        time_symbol_table,
                        time_compress_no_symbol,
                        start_compression,
                        end_compression,
                    ) = (-1, -1, -1, -1, -1)

                writer.writerow(
                    {
                        "name": "fsst_without_fst",
                        "input_file": file,
                        "number_lines_to_sample": "NULL",
                        "start": start_time,
                        "duration": time_compress * 1000,  # Convert to milliseconds
                        "duration_symbol_table": time_symbol_table
                        * 1000,  # Convert to milliseconds
                        "duration_compress_no_symbol": time_compress_no_symbol
                        * 1000,  # Convert to milliseconds
                        "input_size": input_size,
                        "output_size": output_size,
                        "compression_ratio": compression_ratio,
                    }
                )
                csvfile.flush()

                # fsst_with_fst_and_fsst_sampling run
                start_time = time.time()

                # Print current file
                print(
                    "Running",
                    file,
                    "with FST and FSST sampling (fsst_with_fst_and_fsst_sampling)",
                )

                try:
                    compressed_with_fst_and_fsst_sampling_file = os.path.join(
                        compressed_input_file_dir,
                        "fsst_with_fst_and_fsst_sampling.fsst",
                    )
                    output = run_fsst_with_fst_and_fsst_sampling(
                        input_file, compressed_with_fst_and_fsst_sampling_file
                    )
                    end_time = time.time()
                    input_size, output_size, compression_ratio = extract_sizes(output)
                    (
                        time_compress,
                        time_symbol_table,
                        time_compress_no_symbol,
                        start_compression,
                        end_compression,
                    ) = extract_times(output)
                except subprocess.CalledProcessError:
                    end_time = time.time()
                    input_size, output_size, compression_ratio = -1, -1, -1
                    (
                        time_compress,
                        time_symbol_table,
                        time_compress_no_symbol,
                        start_compression,
                        end_compression,
                    ) = (
                        -1,
                        -1,
                        -1,
                        -1,
                        -1,
                    )

                writer.writerow(
                    {
                        "name": "fsst_with_fst_and_fsst_sampling",
                        "input_file": file,
                        "number_lines_to_sample": "NULL",
                        "start": start_time,
                        "duration": time_compress * 1000,  # Convert to milliseconds
                        "duration_symbol_table": time_symbol_table
                        * 1000,  # Convert to milliseconds
                        "duration_compress_no_symbol": time_compress_no_symbol
                        * 1000,  # Convert to milliseconds
                        "input_size": input_size,
                        "output_size": output_size,
                        "compression_ratio": compression_ratio,
                    }
                )
                csvfile.flush()

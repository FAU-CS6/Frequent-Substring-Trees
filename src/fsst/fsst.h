#ifndef FSST_WITH_FST_INCLUDED_H
#define FSST_WITH_FST_INCLUDED_H

#include "../../lib/fsst/libfsst.hpp"

/**
 * @brief Calibrate a FSST symbol table from a batch of strings.
 * @param n_samples The number of strings in the batch to sample from.
 * @param strIn The string start pointers.
 * @return A pointer to the created FSST encoder.
 */
fsst_encoder_t *fsst_create_with_fst(size_t n_samples, unsigned char *strIn[]);

/**
 * Tries to mimic the original fsst_create function
 */
fsst_encoder_t *fsst_create_with_fst_w_fsst_sampling(size_t n, size_t lenIn[], u8 *strIn[], int zeroTerminated);

#endif
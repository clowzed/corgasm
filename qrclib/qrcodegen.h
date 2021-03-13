#ifndef __CORGASM_QRCLIB_H__
#define __CORGASM_QRCLIB_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define penalty_1   3
#define penalty_2   3
#define penalty_3  40
#define penalty_4  10

#define corgasm_qrclib_minimum_version   1
#define corgasm_qrclib_maximum_version   40

#define corgasm_qrclib_buflen_by_version(version)  ((((version) * 4 + 17) * ((version) * 4 + 17) + 7) / 8 + 1)

#define corgasm_qrclib_buflen_maximum  corgasm_qrclib_buflen_by_version(corgasm_qrclib_maximum_version)

#define corgasm_qrclib_reed_solomon_maximun_degree 30

#define ALPHANUMERIC_CHARSET "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:"

#define iterate(qrcode) int x, y = 0; for (y = 0; y < qrcode[0]; y++) {for (x = 0; x < qrcode[0]; x++){ bool is_black = corgasm_qrclib_get_module(qrcode, x, y);

const int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
	//0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
	{-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
	{-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
	{-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};


const int8_t ECC_CODEWORDS_PER_BLOCK[4][41] = {
	//0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
	{-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
	{-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
	{-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};


typedef enum corgasm_qrclib_ecc
{
	low      = 0,
	medium   = 1,
	quartile = 2,
	high     = 3,
}qr_ecc;


typedef enum corgasm_qrclib_mask
{
	mask_auto = -1,
	mask_0    = 0,
	mask_1    = 1,
	mask_2    = 2,
	mask_3    = 3,
	mask_4    = 4,
	mask_5    = 5,
	mask_6    = 6,
	mask_7    = 7,
} qr_mask;


typedef enum corgasm_qrclib_mode {
	numeric      = 0x1,
	alnumeric    = 0x2,
	byte         = 0x4,
	kanji        = 0x8,
	eci          = 0x7,
}qr_mode;


typedef struct corgasm_qrclib_segment {
	qr_mode   mode;
	uint8_t * data;

	int    bitlen;
	int    length;
}qr_segment;

typedef struct corgasm_qrclib_functions
{
	bool 	(*encode)  (const char * text, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename));
	bool 	(*save_ppm)(uint8_t ** data, int size, const char * filename);
	uint8_t (*size)    (const uint8_t * datam);
	bool    (*wifi)    (const char * authentication_type, const char * ssid, const char * password, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename));
}corgasm_qrclib_functions;

bool    corgasm_qrclib_encode(const char * text, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename));
bool    corgasm_qrclib_save_ppm(uint8_t ** data, int size, const char * filename);
uint8_t corgasm_qrclib_size(const uint8_t * qrcode);
bool    corgasm_qrclib_wifi(const char * authentication_type, const char * ssid, const char * password, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename));
uint8_t ** corgasm_qrclib_upscale(uint8_t * data, int * size);


static const corgasm_qrclib_functions qrclib = {
	.encode   = corgasm_qrclib_encode,
	.save_ppm = corgasm_qrclib_save_ppm,
	.size     = corgasm_qrclib_size,
	.wifi     = corgasm_qrclib_wifi,
};


bool corgasm_qrclib_encode_text(const char * text,
								uint8_t *    buffer,
								uint8_t *    qrcode,
								qr_ecc       ecl,
								int          minimum_version,
								int          maximum_version,
								qr_mask      mask,
								bool         boost_ecl);


bool corgasm_qrclib_encode_binary(uint8_t * temp_and_data,
								  size_t    data_len,
								  uint8_t * qrcode,
								  qr_ecc    ecl,
								  int       minimum_version,
								  int       maximum_version,
								  qr_mask   mask,
								  bool      boost_ecl);


bool corgasm_qrclib_encode_segments(const qr_segment * segments,
									size_t             len,
									qr_ecc             ecl,
									uint8_t *          buffer,
									uint8_t *          qrcode);


bool corgasm_qrclib_encode_segments_advanced(const qr_segment * segments,
											size_t              len,
											qr_ecc              ecl,
											int                 minimum_version,
											int                 maximum_version,
											qr_mask             mask,
											bool                boost_ecl,
											uint8_t *           buffer,
											uint8_t *           qrcode);


bool corgasm_qrclib_is_alphanumeric(const char * text);

bool corgasm_qrclib_is_numeric(const char * text);


size_t corgasm_qrclib_get_segment_buffer_size(qr_mode mode, size_t length);


qr_segment corgasm_qrclib_make_bytes(const uint8_t * data, size_t length, uint8_t * buffer);


qr_segment corgasm_qrclib_make_numeric(const char * digits, uint8_t * buffer);


qr_segment corgasm_qrclib_make_alnumeric(const char * text, uint8_t * buffer);


qr_segment corgasm_qrclib_make_eci(long assign_value, uint8_t * buffer);


bool corgasm_qrclib_is_black(const uint8_t * qrcode, int x, int y);


void corgasm_qrclib_append_bits_to_buffer(int value, int number_of_bits, uint8_t * buffer, int * bit_len);


void corgasm_qrclib_add_ecc_and_interleave(uint8_t * data, int version, qr_ecc ecl, uint8_t * result);


int corgasm_qrclib_get_number_of_codewords(int version, qr_ecc ecl);


int corgasm_qrclib_get_raw_data_modules(int version);


void corgasm_qrclib_reed_solomon_divisor(int degree, uint8_t * result);


void corgasm_qrclib_reed_solomon_remainder(const uint8_t * data,
										   int             data_length,
								 		   const uint8_t * generator,
								           int             degree,
								           uint8_t *       result);


uint8_t corgasm_qrclib_reed_solomon_multiply(uint8_t x, uint8_t y);


void corgasm_qrclib_initialize_function_modules(int version, uint8_t * qrcode);


void corgasm_qrclib_draw_white_function_modules(uint8_t * qrcode, int version);


void corgasm_qrclib_draw_format_bits(qr_ecc ecl, qr_mask mask, uint8_t * qrcode);


int corgasm_qrclib_get_alignment_pattern_positions(int version, uint8_t * result);


void corgasm_qrclib_fill_rectangle(int left, int top, int width, int height, uint8_t * qrcode);


void corgasm_qrclib_draw_codewords(const uint8_t * data, int data_length, uint8_t * qrcode);


void corgasm_qrclib_apply_mask(uint8_t * function_modules, uint8_t * qrcode, qr_mask mask);


long corgasm_qrclib_get_penalty_score(const uint8_t * qrcode);


int corgasm_qrclib_find_penalty_count_patterns(const int * running_history);


int corgasm_qrclib_find_penalty_terminate_and_count(bool current_color, int current_length, int * running_history, int qrsize);


void corgasm_qrclib_finder_penalty_add_history(int current_length, int * running_history, int qrsize);


bool corgasm_qrclib_get_module(const uint8_t * qrcode, int x, int y);


void corgasm_qrclib_set_color(uint8_t * qrcode, int x, int y, bool is_black);


void corgasm_qrclib_set_color_bounded(uint8_t * qrcode, int x, int y, bool is_black);


bool corgasm_qrclib_bit_is_setted(int x, int i);


int corgasm_qrclib_calculate_segment_bit_length(qr_mode mode, size_t length);


int corgasm_qrclib_get_total_bits(const qr_segment * segments, size_t length, int version);


int corgasm_qrclib_character_bit_width(qr_mode mode, int version);

#endif

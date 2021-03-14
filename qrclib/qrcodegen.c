#include "qrcodegen.h"

bool corgasm_qrclib_encode_text(const char * text,
								uint8_t *    buffer,
								uint8_t *    qrcode,
								qr_ecc       ecl,
								int          minimum_version,
								int          maximum_version,
								qr_mask      mask,
								bool         boost_ecl)
{

	bool was_encoded = true;
	size_t text_length = strlen(text);

	if (text_length == 0)
		return corgasm_qrclib_encode_segments_advanced(NULL, 0, ecl, minimum_version, maximum_version, mask, boost_ecl, buffer, qrcode);

	size_t buffer_length = corgasm_qrclib_buflen_maximum;

	qr_segment segment = {
		.mode   = byte,
		.data   = NULL,
		.bitlen = 0,
		.length = 0
		};

	if (corgasm_qrclib_is_numeric(text))
	{
		if (corgasm_qrclib_get_segment_buffer_size((int)numeric, text_length) > buffer_length)
			was_encoded = false;
		else
			segment = corgasm_qrclib_make_numeric(text, buffer);
	}
	else if (corgasm_qrclib_is_alphanumeric(text))
	{
		if (corgasm_qrclib_get_segment_buffer_size((int)alnumeric, text_length) > buffer_length)
			was_encoded = false;
		else
			segment = corgasm_qrclib_make_alnumeric(text, buffer);
	}
	else
	{
		if (text_length > buffer_length)
			was_encoded = false;
		else
		{
			for (size_t i = 0; i < text_length; i++)
				buffer[i] = (uint8_t)text[i];

			segment.mode   = byte;
			segment.bitlen = corgasm_qrclib_calculate_segment_bit_length(segment.mode, text_length);
			if ((int)segment.bitlen == -1)
				was_encoded = false;
			else
			{
				segment.length = text_length;
				segment.data   = buffer;
			}
		}
	}
	if (was_encoded)
		return corgasm_qrclib_encode_segments_advanced(&segment, 1, ecl, minimum_version, maximum_version, mask, boost_ecl, buffer, qrcode);
	else
		qrcode[0] = 0;

	return was_encoded;
}




bool corgasm_qrclib_encode_binary(uint8_t * temp_and_data,
								  size_t    data_len,
								  uint8_t * qrcode,
								  qr_ecc    ecl,
								  int       minimum_version,
								  int       maximum_version,
								  qr_mask   mask,
								  bool      boost_ecl)
{

	qr_segment segment = {
		.mode   = byte,
		.bitlen = corgasm_qrclib_calculate_segment_bit_length(segment.mode, data_len),
		.length = 0,
		.data   = NULL,
	};

	if (segment.bitlen == -1)
	{
		qrcode[0] = 0;
		return false;
	}
	segment.length = data_len;
	segment.data   = temp_and_data;
	return corgasm_qrclib_encode_segments_advanced(&segment, 1, ecl, minimum_version, maximum_version, mask, boost_ecl, temp_and_data, qrcode);
}


void corgasm_qrclib_append_bits_to_buffer(int value, int number_of_bits, uint8_t * buffer, int * bit_len)
{
	for (int i = number_of_bits - 1; i >= 0; i--, (*bit_len)++)
		buffer[*bit_len >> 3] |= ((value >> i) & 1) << (7 - (*bit_len & 7));
}

bool corgasm_qrclib_encode_segments(const qr_segment * segments,
									size_t             len,
									qr_ecc             ecl,
									uint8_t *          buffer,
									uint8_t *          qrcode)
{
	return corgasm_qrclib_encode_segments_advanced(segments, len, ecl, corgasm_qrclib_minimum_version, corgasm_qrclib_maximum_version, mask_auto, true, buffer, qrcode);
}

bool corgasm_qrclib_encode_segments_advanced(const qr_segment * segments,
											size_t              len,
											qr_ecc              ecl,
											int                 minimum_version,
											int                 maximum_version,
											qr_mask             mask,
											bool                boost_ecl,
											uint8_t *           buffer,
											uint8_t *           qrcode)
{

	int version = minimum_version, data_used_bits = 0;

	for (version = minimum_version;; version++)
	{
		int data_capacity_bits = corgasm_qrclib_get_number_of_codewords(version, ecl) * 8;
		data_used_bits         = corgasm_qrclib_get_total_bits(segments, len, version);

		if (data_used_bits != -1 && data_used_bits <= data_capacity_bits)
			break;

		if (version >= maximum_version)
		{
			qrcode[0] = 0;
			return false;
		}
	}


	for (int i = medium; i <= high; i++)
	{
		if (boost_ecl && data_used_bits <= corgasm_qrclib_get_number_of_codewords(version, i) * 8)
			ecl = i;
	}

	memset(qrcode, 0, corgasm_qrclib_buflen_by_version(version) * sizeof(qrcode[0]));

	int bit_lenght = 0;
	for (size_t i = 0; i < len; i++)
	{
		const qr_segment * segment = &segments[i];

		corgasm_qrclib_append_bits_to_buffer((unsigned int)segment->mode, 4, qrcode, &bit_lenght);
		corgasm_qrclib_append_bits_to_buffer((unsigned int)segment->length, corgasm_qrclib_character_bit_width(segment->mode, version), qrcode, &bit_lenght);

		for (int j = 0; j < (int)segment->bitlen; j++)
		{
			int bit = (segment->data[j >> 3] >> (7 - (j & 7))) & 1;
			corgasm_qrclib_append_bits_to_buffer((unsigned int)bit, 1, qrcode, &bit_lenght);
		}
	}

	int data_capacity_bits = corgasm_qrclib_get_number_of_codewords(version, ecl) * 8;
	int terminator_bits = data_capacity_bits - bit_lenght;

	if (terminator_bits > 4) terminator_bits = 4;

	corgasm_qrclib_append_bits_to_buffer(0, terminator_bits, qrcode, &bit_lenght);
	corgasm_qrclib_append_bits_to_buffer(0, (8 - bit_lenght % 8) % 8, qrcode, &bit_lenght);

	for (uint8_t pad_byte = 0xEC; bit_lenght < data_capacity_bits; pad_byte ^= 0xEC ^ 0x11)
		corgasm_qrclib_append_bits_to_buffer(pad_byte, 8, qrcode, &bit_lenght);


	corgasm_qrclib_add_ecc_and_interleave(qrcode, version, ecl, buffer);
	corgasm_qrclib_initialize_function_modules(version, qrcode);
	corgasm_qrclib_draw_codewords(buffer, corgasm_qrclib_get_raw_data_modules(version) / 8, qrcode);
	corgasm_qrclib_draw_white_function_modules(qrcode, version);
	corgasm_qrclib_initialize_function_modules(version, buffer);

	if (mask == mask_auto)
	{
		long min_penalty = LONG_MAX;

		for (int i = 0; i < 8; i++)
		{
			qr_mask current_mask = i;

			corgasm_qrclib_apply_mask(buffer, qrcode, current_mask);

			corgasm_qrclib_draw_format_bits(ecl, current_mask, qrcode);

			long penalty = corgasm_qrclib_get_penalty_score(qrcode);
			if (penalty < min_penalty)
			{
				mask = current_mask;
				min_penalty = penalty;
			}
			corgasm_qrclib_apply_mask(buffer, qrcode, current_mask);
		}
	}

	corgasm_qrclib_apply_mask(buffer, qrcode, mask);

	corgasm_qrclib_draw_format_bits(ecl, mask, qrcode);

	return true;
}

void corgasm_qrclib_add_ecc_and_interleave(uint8_t * data, int version, qr_ecc ecl, uint8_t * result)
{
	int number_of_blocks = NUM_ERROR_CORRECTION_BLOCKS[(int)ecl][version];
	int block_ecc_len      = ECC_CODEWORDS_PER_BLOCK[(int)ecl][version];

	int raw_codewords = corgasm_qrclib_get_raw_data_modules(version) / 8;
	int data_len      = corgasm_qrclib_get_number_of_codewords(version, ecl);

	int num_short_blocks     = number_of_blocks - raw_codewords % number_of_blocks;
	int short_block_data_len = raw_codewords / number_of_blocks - block_ecc_len;


	uint8_t rsdiv[corgasm_qrclib_reed_solomon_maximun_degree];
	corgasm_qrclib_reed_solomon_divisor(block_ecc_len, rsdiv);

	const uint8_t * dat = data;

	for (int i = 0; i < number_of_blocks; i++)
	{
		int dat_len = short_block_data_len + (i < num_short_blocks ? 0 : 1);
		uint8_t * ecc = &data[data_len];

		corgasm_qrclib_reed_solomon_remainder(dat, dat_len, rsdiv, block_ecc_len, ecc);

		for (int j = 0, k = i; j < dat_len; j++, k += number_of_blocks)
		{
			if (j == short_block_data_len)
				k -= num_short_blocks;
			result[k] = dat[j];
		}

		for (int j = 0, k = data_len + i; j < block_ecc_len; j++, k += number_of_blocks)
			result[k] = ecc[j];
		dat += dat_len;
	}
}

int corgasm_qrclib_get_number_of_codewords(int version, qr_ecc ecl)
{
	int v = version, e = (int)ecl;
	return corgasm_qrclib_get_raw_data_modules(v) / 8 - ECC_CODEWORDS_PER_BLOCK[e][v] * NUM_ERROR_CORRECTION_BLOCKS[e][v];
}

int corgasm_qrclib_get_raw_data_modules(int version)
{
	int result = (16 * version + 128) * version + 64;

	if (version >= 2)
	{
		int number_align = version / 7 + 2;
		result -= (25 * number_align - 10) * number_align - 55;
		if (version >= 7)
			result -= 36;
	}
	return result;
}

void corgasm_qrclib_reed_solomon_divisor(int degree, uint8_t * result)
{
	memset(result, 0, (size_t)degree * sizeof(result[0]));
	result[degree - 1] = 1;

	uint8_t root = 1;
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree; j++)
		{
			result[j] = corgasm_qrclib_reed_solomon_multiply(result[j], root);
			if (j + 1 < degree)
				result[j] ^= result[j + 1];
		}
		root = corgasm_qrclib_reed_solomon_multiply(root, 0x02);
	}
}

void corgasm_qrclib_reed_solomon_remainder(const uint8_t * data,
										   int             data_length,
								 		   const uint8_t * generator,
								           int             degree,
								           uint8_t *       result)
{
	memset(result, 0, (size_t)degree * sizeof(result[0]));
	for (int i = 0; i < data_length; i++)
	{
		uint8_t factor = data[i] ^ result[0];

		memmove(&result[0], &result[1], (size_t)(degree - 1) * sizeof(result[0]));

		result[degree - 1] = 0;

		for (int j = 0; j < degree; j++)
			result[j] ^= corgasm_qrclib_reed_solomon_multiply(generator[j], factor);
	}
}


uint8_t corgasm_qrclib_reed_solomon_multiply(uint8_t x, uint8_t y)
{
	uint8_t z = 0;
	for (int i = 7; i >= 0; i--)
	{
		z = (uint8_t)((z << 1) ^ ((z >> 7) * 0x11D));
		z ^= ((y >> i) & 1) * x;
	}
	return z;
}


void corgasm_qrclib_initialize_function_modules(int version, uint8_t * qrcode)
{
	int qrsize = version * 4 + 17;
	memset(qrcode, 0, (size_t)((qrsize * qrsize + 7) / 8 + 1) * sizeof(qrcode[0]));

	qrcode[0] = (uint8_t)qrsize;

	corgasm_qrclib_fill_rectangle(6, 0, 1, qrsize, qrcode);
	corgasm_qrclib_fill_rectangle(0, 6, qrsize, 1, qrcode);

	corgasm_qrclib_fill_rectangle(0, 0, 9, 9, qrcode);
	corgasm_qrclib_fill_rectangle(qrsize - 8, 0, 8, 9, qrcode);
	corgasm_qrclib_fill_rectangle(0, qrsize - 8, 9, 8, qrcode);

	uint8_t align_pattern_positon[7];
	int positions = corgasm_qrclib_get_alignment_pattern_positions(version, align_pattern_positon);
	for (int i = 0; i < positions; i++)
	{
		for (int j = 0; j < positions; j++)
		{
			// Don't draw on the three finder corners
			if (!((i == 0 && j == 0) || (i == 0 && j == positions - 1) || (i == positions - 1 && j == 0)))
				corgasm_qrclib_fill_rectangle(align_pattern_positon[i] - 2, align_pattern_positon[j] - 2, 5, 5, qrcode);
		}
	}

	// Fill version blocks
	if (version >= 7)
	{
		corgasm_qrclib_fill_rectangle(qrsize - 11, 0, 3, 6, qrcode);
		corgasm_qrclib_fill_rectangle(0, qrsize - 11, 6, 3, qrcode);
	}
}

void corgasm_qrclib_draw_white_function_modules(uint8_t qrcode[], int version)
{
	int qrsize = corgasm_qrclib_size(qrcode);
	for (int i = 7; i < qrsize - 7; i += 2)
	{
		corgasm_qrclib_set_color(qrcode, 6, i, false);
		corgasm_qrclib_set_color(qrcode, i, 6, false);
	}

	for (int dy = -4; dy <= 4; dy++)
	{
		for (int dx = -4; dx <= 4; dx++)
		{
			int dist = abs(dx);
			if (abs(dy) > dist)
				dist = abs(dy);
			if (dist == 2 || dist == 4)
			{
				corgasm_qrclib_set_color_bounded(qrcode, 3 + dx, 3 + dy, false);
				corgasm_qrclib_set_color_bounded(qrcode, qrsize - 4 + dx, 3 + dy, false);
				corgasm_qrclib_set_color_bounded(qrcode, 3 + dx, qrsize - 4 + dy, false);
			}
		}
	}

	uint8_t align_pattern_positon[7];
	int positions = corgasm_qrclib_get_alignment_pattern_positions(version, align_pattern_positon);
	for (int i = 0; i < positions; i++)
	{
		for (int j = 0; j < positions; j++)
		{
			if ((i == 0 && j == 0)             ||
				(i == 0 && j == positions - 1) ||
				(i == positions - 1 && j == 0))
				continue;

			for (int dy = -1; dy <= 1; dy++)
				for (int dx = -1; dx <= 1; dx++)
					corgasm_qrclib_set_color(qrcode, align_pattern_positon[i] + dx, align_pattern_positon[j] + dy, dx == 0 && dy == 0);
		}
	}

	if (version >= 7)
	{
		int rem = version;

		for (int i = 0; i < 12; i++)
			rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);

		long bits = (long)version << 12 | rem;

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int k = qrsize - 11 + j;
				corgasm_qrclib_set_color(qrcode, k, i, (bits & 1) != 0);
				corgasm_qrclib_set_color(qrcode, i, k, (bits & 1) != 0);
				bits >>= 1;
			}
		}
	}
}

void corgasm_qrclib_draw_format_bits(qr_ecc ecl, qr_mask mask, uint8_t * qrcode)
{
	static const int table[] = {1, 0, 3, 2};

	int data = table[(int)ecl] << 3 | (int)mask;
	int rem = data;

	for (int i = 0; i < 10; i++)
		rem = (rem << 1) ^ ((rem >> 9) * 0x537);

	int bits = (data << 10 | rem) ^ 0x5412;

	for (int i = 0; i <= 5; i++)
		corgasm_qrclib_set_color(qrcode, 8, i, corgasm_qrclib_bit_is_setted(bits, i));

	corgasm_qrclib_set_color(qrcode, 8, 7, corgasm_qrclib_bit_is_setted(bits, 6));
	corgasm_qrclib_set_color(qrcode, 8, 8, corgasm_qrclib_bit_is_setted(bits, 7));
	corgasm_qrclib_set_color(qrcode, 7, 8, corgasm_qrclib_bit_is_setted(bits, 8));

	for (int i = 9; i < 15; i++)
		corgasm_qrclib_set_color(qrcode, 14 - i, 8, corgasm_qrclib_bit_is_setted(bits, i));

	int qrsize = corgasm_qrclib_size(qrcode);

	for (int i = 0; i < 8; i++)
		corgasm_qrclib_set_color(qrcode, qrsize - 1 - i, 8, corgasm_qrclib_bit_is_setted(bits, i));

	for (int i = 8; i < 15; i++)
		corgasm_qrclib_set_color(qrcode, 8, qrsize - 15 + i, corgasm_qrclib_bit_is_setted(bits, i));

	corgasm_qrclib_set_color(qrcode, 8, qrsize - 8, true);
}

int corgasm_qrclib_get_alignment_pattern_positions(int version, uint8_t * result)
{
	if (version == 1)
		return 0;

	int positions = version / 7 + 2;
	int step = (version == 32) ? 26 : (version * 4 + positions * 2 + 1) / (positions * 2 - 2) * 2;

	for (int i = positions - 1, pos = version * 4 + 10; i >= 1; i--, pos -= step)
		result[i] = (uint8_t)pos;

	result[0] = 6;
	return positions;
}

void corgasm_qrclib_fill_rectangle(int left, int top, int width, int height, uint8_t * qrcode)
{
	for (int dy = 0; dy < height; dy++)
		for (int dx = 0; dx < width; dx++)
			corgasm_qrclib_set_color(qrcode, left + dx, top + dy, true);
}

void corgasm_qrclib_draw_codewords(const uint8_t * data, int data_length, uint8_t * qrcode)
{
	int qrsize = corgasm_qrclib_size(qrcode);
	int i = 0;
	for (int right = qrsize - 1; right >= 1; right -= 2)
	{
		if (right == 6) right = 5;

		for (int vert = 0; vert < qrsize; vert++)
		{
			for (int j = 0; j < 2; j++)
			{
				int x = right - j;
				bool upward = !((right + 1) & 2);
				int y = upward ? qrsize - 1 - vert : vert;

				if (!corgasm_qrclib_get_module(qrcode, x, y) && i < data_length * 8)
				{
					bool black = corgasm_qrclib_bit_is_setted(data[i >> 3], 7 - (i & 7));
					corgasm_qrclib_set_color(qrcode, x, y, black);
					i++;
				}
			}
		}
	}
}

void corgasm_qrclib_apply_mask(uint8_t * function_modules, uint8_t * qrcode, qr_mask mask)
{
	int qrsize = corgasm_qrclib_size(qrcode);

	for (int y = 0; y < qrsize; y++)
	{
		for (int x = 0; x < qrsize; x++)
		{
			if (corgasm_qrclib_get_module(function_modules, x, y))
				continue;

			bool invert = false;

			switch ((int)mask)
			{
			case 0:
				invert = (x + y) % 2 == 0;
				break;
			case 1:
				invert = y % 2 == 0;
				break;
			case 2:
				invert = x % 3 == 0;
				break;
			case 3:
				invert = (x + y) % 3 == 0;
				break;
			case 4:
				invert = (x / 3 + y / 2) % 2 == 0;
				break;
			case 5:
				invert = x * y % 2 + x * y % 3 == 0;
				break;
			case 6:
				invert = (x * y % 2 + x * y % 3) % 2 == 0;
				break;
			case 7:
				invert = ((x + y) % 2 + x * y % 3) % 2 == 0;
				break;
			}
			bool val = corgasm_qrclib_get_module(qrcode, x, y);
			corgasm_qrclib_set_color(qrcode, x, y, val ^ invert);
		}
	}
}

long corgasm_qrclib_get_penalty_score(const uint8_t * qrcode)
{
	int qrsize = corgasm_qrclib_size(qrcode);
	long result = 0;

	for (int y = 0; y < qrsize; y++)
	{
		bool run_color = false;
		int run_x = 0;
		int running_history[7] = {0};

		for (int x = 0; x < qrsize; x++)
		{
			if (corgasm_qrclib_get_module(qrcode, x, y) == run_color)
			{
				run_x++;
				if (run_x == 5)     result += penalty_1;
				else if (run_x > 5) result++;
			}
			else
			{
				corgasm_qrclib_finder_penalty_add_history(run_x, running_history, qrsize);

				if (!run_color)
					result += corgasm_qrclib_find_penalty_count_patterns(running_history) * penalty_3;

				run_color = corgasm_qrclib_get_module(qrcode, x, y);
				run_x = 1;
			}
		}
		result += corgasm_qrclib_find_penalty_terminate_and_count(run_color, run_x, running_history, qrsize) * penalty_3;
	}


	for (int x = 0; x < qrsize; x++)
	{
		bool run_color = false;
		int run_y = 0;
		int running_history[7] = {0};
		for (int y = 0; y < qrsize; y++)
		{
			if (corgasm_qrclib_get_module(qrcode, x, y) == run_color)
			{
				run_y++;
				if (run_y == 5)     result += penalty_1;
				else if (run_y > 5) result++;
			}
			else
			{
				corgasm_qrclib_finder_penalty_add_history(run_y, running_history, qrsize);

				if (!run_color)
					result += corgasm_qrclib_find_penalty_count_patterns(running_history) * penalty_3;

				run_color = corgasm_qrclib_get_module(qrcode, x, y);
				run_y = 1;
			}
		}
		result += corgasm_qrclib_find_penalty_terminate_and_count(run_color, run_y, running_history, qrsize) * penalty_3;
	}

	for (int y = 0; y < qrsize - 1; y++)
	{
		for (int x = 0; x < qrsize - 1; x++)
		{
			bool color = corgasm_qrclib_get_module(qrcode, x, y);
			if (color == corgasm_qrclib_get_module(qrcode, x + 1, y) &&
				color == corgasm_qrclib_get_module(qrcode, x, y + 1) &&
				color == corgasm_qrclib_get_module(qrcode, x + 1, y + 1))
				result += penalty_2;
		}
	}

	int black = 0;

	for (int y = 0; y < qrsize; y++)
	{
		for (int x = 0; x < qrsize; x++)
		{
			if (corgasm_qrclib_get_module(qrcode, x, y))
				black++;
		}
	}


	int total = qrsize * qrsize;

	int k = (int)((labs(black * 20L - total * 10L) + total - 1) / total) - 1;
	result += k * penalty_4;
	return result;
}

int corgasm_qrclib_find_penalty_count_patterns(const int * running_history)
{
	int n = running_history[1];
	bool core = n > 0 && running_history[2] == n &&
						 running_history[3] == n * 3 &&
						 running_history[4] == n &&
						 running_history[5] == n;
	return  (core && running_history[0] >= n * 4 &&
					 running_history[6] >= n ? 1 : 0) +
			(core && running_history[6] >= n * 4 &&
			         running_history[0] >= n ? 1 : 0);
}

int corgasm_qrclib_find_penalty_terminate_and_count(bool current_color, int current_length, int * running_history, int qrsize)
{
	if (current_color)
	{
		corgasm_qrclib_finder_penalty_add_history(current_length, running_history, qrsize);
		current_length = 0;
	}
	current_length += qrsize;
	corgasm_qrclib_finder_penalty_add_history(current_length, running_history, qrsize);
	return corgasm_qrclib_find_penalty_count_patterns(running_history);
}

void corgasm_qrclib_finder_penalty_add_history(int currentRunLength, int * running_history, int qrsize)
{
	if (running_history[0] == 0)
		currentRunLength += qrsize;
	memmove(&running_history[1], &running_history[0], 6 * sizeof(running_history[0]));
	running_history[0] = currentRunLength;
}

uint8_t corgasm_qrclib_size(const uint8_t * qrcode)
{
	return qrcode[0];
}

bool corgasm_qrclib_is_black(const uint8_t * qrcode, int x, int y)
{
	int qrsize = qrcode[0];
	return (0 <= x && x < qrsize && 0 <= y && y < qrsize) && corgasm_qrclib_get_module(qrcode, x, y);
}

bool corgasm_qrclib_get_module(const uint8_t * qrcode, int x, int y)
{
	int qrsize = qrcode[0];
	int index  = y * qrsize + x;
	return corgasm_qrclib_bit_is_setted(qrcode[(index >> 3) + 1], index & 7);
}

void corgasm_qrclib_set_color(uint8_t * qrcode, int x, int y, bool is_black)
{
	int qrsize = qrcode[0];
	int index  = y * qrsize + x;

	int bit_index  = index & 7;
	int byte_index = (index >> 3) + 1;

	if (is_black)
		qrcode[byte_index] |= 1 << bit_index;
	else
		qrcode[byte_index] &= (1 << bit_index) ^ 0xFF;
}

void corgasm_qrclib_set_color_bounded(uint8_t * qrcode, int x, int y, bool is_black)
{
	int qrsize = qrcode[0];
	if (0 <= x && x < qrsize && 0 <= y && y < qrsize)
		corgasm_qrclib_set_color(qrcode, x, y, is_black);
}

bool corgasm_qrclib_bit_is_setted(int value, int index)
{
	return ((value >> index) & 1) != 0;
}

bool corgasm_qrclib_is_alphanumeric(const char *text)
{
	for (; *text != '\0'; text++)
		if (!strchr(ALPHANUMERIC_CHARSET, *text))
			return false;
	return true;
}

bool corgasm_qrclib_is_numeric(const char *text)
{
	for (; *text != '\0'; text++)
		if (*text < '0' || *text > '9')
			return false;
	return true;
}

size_t corgasm_qrclib_get_segment_buffer_size(qr_mode mode, size_t length)
{
	int temp = corgasm_qrclib_calculate_segment_bit_length(mode, length);
	if (temp == -1) return SIZE_MAX;
	return ((size_t)temp + 7) / 8;
}

int corgasm_qrclib_calculate_segment_bit_length(qr_mode mode, size_t length)
{
	if (length > (unsigned int)INT16_MAX)
		return -1;
	long result = (long)length;

	if (mode == numeric)
		result = (result * 10 + 2) / 3;
	else if (mode == alnumeric)
		result = (result * 11 + 1) / 2;
	else if (mode == byte)
		result *= 8;
	else if (mode == kanji)
		result *= 13;
	else if (mode == eci && length == 0)
		result = 3 * 8;
	else
		return -1;

	if (result > INT16_MAX)
		return -1;

	return (int)result;
}

qr_segment corgasm_qrclib_make_bytes(const uint8_t * data, size_t length, uint8_t * buffer)
{
	qr_segment result;
	result.mode = byte;
	result.bitlen = corgasm_qrclib_calculate_segment_bit_length(result.mode, length);
	result.length = length;
	if (length > 0) memcpy(buffer, data, length * sizeof(uint8_t));
	result.data = buffer;
	return result;
}

qr_segment corgasm_qrclib_make_numeric(const char * digits, uint8_t * buffer)
{

	size_t len    = strlen(digits);
	qr_segment result = {
		.mode   = numeric,
		.data   = NULL,
		.length = len,
		.bitlen = 0
		};

	int bit_len   = corgasm_qrclib_calculate_segment_bit_length(result.mode, len);

	if (bit_len > 0)
		memset(buffer, 0, ((size_t)bit_len + 7) / 8 * sizeof(uint8_t));

	result.bitlen = 0;

	unsigned int accum_data = 0;
	int accum_count = 0;
	for (; *digits != '\0'; digits++)
	{
		char c = *digits;
		accum_data = accum_data * 10 + (unsigned int)(c - '0');
		accum_count++;
		if (accum_count == 3)
		{
			corgasm_qrclib_append_bits_to_buffer(accum_data, 10, buffer, &result.bitlen);
			accum_data = 0;
			accum_count = 0;
		}
	}
	if (accum_count > 0)
		corgasm_qrclib_append_bits_to_buffer(accum_data, accum_count * 3 + 1, buffer, &result.bitlen);

	result.data = buffer;
	return result;
}

qr_segment corgasm_qrclib_make_alnumeric(const char *text, uint8_t * buffer)
{
	size_t len = strlen(text);
	qr_segment result = {
		.mode   = alnumeric,
		.data   = NULL,
		.length = len,
		.bitlen = 0
	};
	int bitLen = corgasm_qrclib_calculate_segment_bit_length(result.mode, len);

	if (bitLen > 0)
		memset(buffer, 0, ((size_t)bitLen + 7) / 8 * sizeof(uint8_t));

	unsigned int accum_data = 0;
	int accum_count = 0;
	for (; *text != '\0'; text++)
	{
		const char *temp = strchr(ALPHANUMERIC_CHARSET, *text);
		accum_data = accum_data * 45 + (unsigned int)(temp - ALPHANUMERIC_CHARSET);
		accum_count++;
		if (accum_count == 2)
		{
			corgasm_qrclib_append_bits_to_buffer(accum_data, 11, buffer, &result.bitlen);
			accum_data = 0;
			accum_count = 0;
		}
	}

	if (accum_count > 0)
		corgasm_qrclib_append_bits_to_buffer(accum_data, 6, buffer, &result.bitlen);

	result.data = buffer;
	return result;
}

qr_segment corgasm_qrclib_make_eci(long assign_value, uint8_t * buffer)
{
	qr_segment result = {
		.mode   = eci,
		.length = 0,
		.bitlen = 0,
		.data   = NULL,
	};

	if (assign_value < (1 << 7))
	{
		memset(buffer, 0, 1 * sizeof(uint8_t));
		corgasm_qrclib_append_bits_to_buffer((unsigned int)assign_value, 8, buffer, &result.bitlen);
	}
	else if (assign_value < (1 << 14))
	{
		memset(buffer, 0, 2 * sizeof(uint8_t));
		corgasm_qrclib_append_bits_to_buffer(2, 2, buffer, &result.bitlen);
		corgasm_qrclib_append_bits_to_buffer((unsigned int)assign_value, 14, buffer, &result.bitlen);
	}
	else if (assign_value < 1000000L)
	{
		memset(buffer, 0, 3 * sizeof(uint8_t));
		corgasm_qrclib_append_bits_to_buffer(6, 3, buffer, &result.bitlen);
		corgasm_qrclib_append_bits_to_buffer((unsigned int)(assign_value >> 10), 11, buffer, &result.bitlen);
		corgasm_qrclib_append_bits_to_buffer((unsigned int)(assign_value & 0x3FF), 10, buffer, &result.bitlen);
	}

	result.data = buffer;
	return result;
}

int corgasm_qrclib_get_total_bits(const qr_segment * segments, size_t length, int version)
{
	long result = 0;
	for (size_t i = 0; i < length; i++)
	{
		int length    = segments[i].length;
		int bitLength = segments[i].bitlen;

		int ccbits = corgasm_qrclib_character_bit_width(segments[i].mode, version);

		if (length >= (1L << ccbits))
			return -1;
		result += 4L + ccbits + bitLength;

		if (result > INT16_MAX)
			return -1;
	}
	return result;
}

int corgasm_qrclib_character_bit_width(qr_mode mode, int version)
{
	int i = (version + 7) / 17;
	switch (mode)
	{
	case numeric:
	{
		static const int temp[] = {10, 12, 14};
		return temp[i];
	}
	case alnumeric:
	{
		static const int temp[] = {9, 11, 13};
		return temp[i];
	}
	case byte:
	{
		static const int temp[] = {8, 16, 16};
		return temp[i];
	}
	case kanji:
	{
		static const int temp[] = {8, 10, 12};
		return temp[i];
	}
	case eci:
		return 0;
	default:
		return -1;
	}
}

bool corgasm_qrclib_encode(const char * text, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename))
{
	bool was_encoded = false;
	if (text && filename && writer)
	{
		size_t    buflen    = corgasm_qrclib_buflen_maximum;
		uint8_t * qrcode    = malloc(buflen * sizeof(uint8_t));
		uint8_t * buffer    = malloc(buflen * sizeof(uint8_t));

		if (!qrcode || !buffer)
		{
			if (qrcode) free(qrcode);
			if (buffer) free(buffer);
		}
		else
		{
			was_encoded = corgasm_qrclib_encode_text(text,
													 buffer,
													 qrcode,
													 medium,
													 corgasm_qrclib_minimum_version,
													 corgasm_qrclib_maximum_version,
													 mask_auto,
													 true);
			if (was_encoded)
			{
				uint8_t ** upscaled = corgasm_qrclib_upscale(qrcode, &size);
				printf("!size: %d\n", size);

				was_encoded = writer(upscaled, size, filename);
				for (int i = 0; i < size; i++)
					free(upscaled[i]);
				free(upscaled);
			}

			free(qrcode);
			free(buffer);
		}
	}
	return was_encoded;
}

bool corgasm_qrclib_save_ppm(uint8_t ** qrcode, int size, const char * filename)
{
	bool was_saved = false;
	if (qrcode && filename)
	{
		FILE * fp = fopen(filename, "wb");
		if (fp)
		{
			fprintf(fp, "P6\n");
			fprintf(fp, "%d %d\n", size, size);
			fprintf(fp, "255\n");
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					bool is_black = qrcode[i][j];
					unsigned char color[3];

					int color_ = is_black ? 0 : 255;

					color[0] = color_;
					color[1] = color_;
					color[2] = color_;

					fwrite(color, 1, 3, fp);
				}
			}
			fprintf(fp, "\n");
			fclose(fp);
			was_saved = true;
		}
	}
	return was_saved;
}

int corgasm_qrclib_upscale_bitmap_size(uint8_t * data, int required_size)
{
	int size = qrclib.size(data);
	return (required_size / size) * size;
}

void corgasm_qrclib_bitmap_fill_rectangle(uint8_t ** bitmap, int row, int col, int pixels_per_pixel, bool is_black)
{
	for (int i = 0; i < pixels_per_pixel; i++)
		for (int j = 0; j < pixels_per_pixel; j++)
			bitmap[row * pixels_per_pixel + i][col * pixels_per_pixel + j] = (is_black);
}

uint8_t ** corgasm_qrclib_upscale(uint8_t * data, int * size)
{
	int bitmap_size = corgasm_qrclib_upscale_bitmap_size(data, *size);
	int pixels_per_pixel = bitmap_size / data[0];
	printf("pixels_per_pixel: %d\n", pixels_per_pixel);
	*size = bitmap_size;
	uint8_t **	bitmap = malloc(sizeof(uint8_t *) * bitmap_size);

	for (int i = 0; i < bitmap_size; i++)
		bitmap[i] = malloc(sizeof(uint8_t) * bitmap_size);


	int old_size = data[0];

	for(int row = 0; row < old_size; ++row)
	{
		for(int col = 0; col < old_size; ++col)
		{
			bool is_black =	corgasm_qrclib_get_module(data, row, col);
			corgasm_qrclib_bitmap_fill_rectangle(bitmap, row, col, pixels_per_pixel ,is_black);
		}
	}
	return bitmap;
}

bool corgasm_qrclib_wifi(const char * authentication_type, const char * ssid, const char * password, const char * filename, int size, bool (*writer)(uint8_t ** data, int size, const char * filename))
{
	bool was_encoded = false;
	if (authentication_type && ssid && password && filename && writer)
	{
		char buffer[1024] = "\0";
		snprintf(buffer, 1024, "WIFI:T:%sS:%s;P:%s;", authentication_type, ssid, password);
		was_encoded = qrclib.encode(buffer, filename, size, writer);
	}
	return was_encoded;
}


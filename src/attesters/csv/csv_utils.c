/* Copyright (c) 2022 Hygon Corporation
 * Copyright (c) 2020-2022 Alibaba Cloud
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <rats-tls/log.h>
#include <openssl/evp.h>

void gen_random_bytes(void *buf, size_t len)
{
	uint32_t i;
	uint8_t *buf_byte = (uint8_t *)buf;

	for (i = 0; i < len; i++)
		buf_byte[i] = rand() & 0xFF;
}

int sm3_hash(const unsigned char *data, size_t len, unsigned char *hash, size_t expected_hash_len)
{
	EVP_MD_CTX *evp_md_ctx = EVP_MD_CTX_new();
	const EVP_MD *evp_md = EVP_sm3();
	int sm3_out_size = 0;
	int ret = -1;

	if (evp_md_ctx == NULL)
		return ret;

	if (!EVP_DigestInit_ex(evp_md_ctx, evp_md, NULL))
		goto err_free_md_ctx;

	if (!EVP_DigestUpdate(evp_md_ctx, data, len))
		goto err_free_md_ctx;

	if (!EVP_DigestFinal_ex(evp_md_ctx, hash, &sm3_out_size))
		goto err_free_md_ctx;

	if (sm3_out_size != expected_hash_len)
		goto err_free_md_ctx;

	ret = 0;

err_free_md_ctx:
	EVP_MD_CTX_free(evp_md_ctx);

	return ret;
}

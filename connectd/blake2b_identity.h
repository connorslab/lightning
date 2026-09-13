#ifndef LIGHTNING_CONNECTD_BLAKE2B_IDENTITY_H
#define LIGHTNING_CONNECTD_BLAKE2B_IDENTITY_H
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Versioned consensus-family declaration, not a software version or secret.
 * The ordinary init networks field independently identifies the chain. */
#define BLAKE2B_IDENTITY "bitcoin-blake2b/header-v2/1"

static inline bool blake2b_identity_matches(const void *data, size_t len)
{
	return data && len == sizeof(BLAKE2B_IDENTITY) - 1
		&& memcmp(data, BLAKE2B_IDENTITY, len) == 0;
}
#endif /* LIGHTNING_CONNECTD_BLAKE2B_IDENTITY_H */

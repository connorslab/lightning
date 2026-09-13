# Paperclip development baseline

Target: Ubuntu 24.04 amd64. This is a development rebuild, not a new protocol
release, reproducible-build claim or replacement signed upstream release.
SOURCE-COMMIT identifies the source; SHA256SUMS checks archive integrity but
is not a publisher signature. Runtime dependencies must be installed separately.
No production installation is performed by this build.

## Compared with v26.06.7-blake2b.2

Runtime source is unchanged. Additions are build automation and documentation.
The experimental mandatory 4242/4243 feature and custom identity TLV have been
withdrawn. This package does not implement a replacement feature, unified channel
signatures, migration orchestration or invoice network isolation. Do not describe
it as migration-compliant or as a safe solution for dual-chain channel exposure.

## How the existing handshake works

1. Lightning's encrypted transport authenticates the peer's node public key.
2. Peers exchange init messages containing feature vectors and a networks TLV.
3. Unsupported required (even) features are rejected. Unknown optional (odd)
   features can be ignored under the normal feature-negotiation rules.
4. If a networks list is supplied with no common configured genesis hash,
   connectd rejects the connection. An omitted list is not rejected solely for
   being absent. A shared genesis hash cannot distinguish Blake2b from SHA256.

There is NO new handshake in this build. It retains compatibility with legacy
peers, but that alone does not make cross-network channel operation safe.

Future standardized work requires the agreed option_blake2b registry allocation
and detailed migration specification. The intended migration design uses optional
peer/node signaling and required invoice/offer signaling, preserving the shared
chain identity until the coordinated later transition. None of that is silently
substituted with a Paperclip-specific bit in this build.

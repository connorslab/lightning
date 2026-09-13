# Previous peering proposal withdrawn

The custom identity TLV and provisional 4242/4243 mandatory handshake feature
have been withdrawn. Do not deploy earlier commits of this branch as a network
standard. Their tests did not establish migration compatibility.

Commit 390157da restored runtime code to the v26.06.7-blake2b.2 baseline.
The later differentiation proposal is described in BLAKE2B-DIFFERENTIATION.md;
it supersedes that baseline with optional signaling and local policy controls.

Future work must follow an agreed migration specification and feature registry.
The old mandatory 4242/4243 bit and custom wire message remain withdrawn.
Production nodes were not upgraded or changed as part of either proposal.

# Previous peering proposal withdrawn

The custom identity TLV and provisional 4242/4243 mandatory handshake feature
have been withdrawn. Do not deploy earlier commits of this branch as a network
standard. Their tests did not establish migration compatibility.

Runtime code on this branch is restored to the v26.06.7-blake2b.2 baseline.
This is a development baseline, not a completed migration implementation or
a claim that the release provides replay-safe channels.

Future work must follow an agreed migration specification and feature registry.
No replacement bit allocation, chain identifier or custom wire message is
introduced here. Existing peer connectivity is preserved. Production nodes
were not upgraded or changed as part of withdrawing this proposal.

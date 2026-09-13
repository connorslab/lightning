# Paperclip changes

Base: privkeyio v26.06.7-blake2b.3.

1. A Blake2b declaration: provisional feature pair 68/69, with optional bit 69
   in init and node announcements. Not a registered BOLT assignment.
2. Unified sighash support: the digest foundation passes 166 Knots vectors.
   Work on transaction-signing integration is in progress. Do not claim channel
   replay protection until that integration and transaction tests pass.

Invoice creation, parsing and payment behavior are unchanged from privkeyio.
There is no custom peer-admission mode or changed genesis identifier.
Cguida's broader plan includes invoice markers and channel migration; those are
outside this narrowed patch. The peer bit is a declaration, not replay protection.

# Unofficial Paperclip test build

Base: privkeyio v26.06.7-blake2b.3, Ubuntu 24.04 amd64.

Scope: a low-number Blake2b peer feature and unified transaction signing.
The unofficial test build includes unified transaction/channel signing and passed
CI run 34790932048. It is for isolated testing with disposable funds, not deployment
on production funded nodes. Existing channels are not automatically migrated.

The peer feature uses provisional 68/69 (advertised required bit 68). Invoices
and genesis are unchanged. See PROPOSAL.md and UNIFIED-PROPOSAL.md for status.

Upstream .3 uses database schema 284; .2 used 282. Any database upgrade requires
separate planning and complete backups. No deployment is performed by this build.

Required bit 68 is an explicit operator-requested departure from cguida's
optional migration signaling. Older peers cannot connect without upgrading.

The release archive is the unchanged CI artifact. Its embedded development notes
predate the final successful tests; the release VERIFICATION.md records the results.

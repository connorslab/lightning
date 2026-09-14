# Unofficial Paperclip test build

Base: privkeyio v26.06.7-blake2b.3, Ubuntu 24.04 amd64.

Scope: required Blake2b peer feature bit 68 and unified transaction signing.
The dedicated build tests pass for peer separation, unified signatures, wallet
transactions, channel funding, payments, restart, closure and splicing. The full
inherited CI suite is still being validated. This is an unofficial test build.

The peer feature uses provisional 68/69 (advertised required bit 68). Invoices
and genesis are unchanged. New Blake2b channels use required channel-type bit 70
for unified signatures, with init capability 71. These assignments are provisional,
not registered BOLT assignments. See [signing scope](BLAKE2B-UNIFIED-PROPOSAL.md).

Upstream .3 uses database schema 284; .2 used 282. Any database upgrade requires
separate planning and complete backups. No deployment is performed by this build.

Required bit 68 is an explicit operator-requested departure from cguida's
optional migration signaling. Older peers cannot connect without upgrading.

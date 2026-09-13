"""Run only against isolated Knots regtest with Blake2b active from height 1."""
from fixtures import *  # noqa: F401,F403
from utils import only_one, wait_for, sync_blockheight


def assert_unified_witnesses(bitcoind, txid, minimum):
    tx = bitcoind.rpc.getrawtransaction(txid, True)
    signatures = []
    for vin in tx['vin']:
        for item in vin.get('txinwitness', []):
            raw = bytes.fromhex(item)
            if len(raw) == 65 or (69 <= len(raw) <= 73 and raw[0] == 0x30):
                signatures.append(raw)
    assert len(signatures) >= minimum
    assert all(s[-1] & 0x20 for s in signatures)
    return tx


def test_unified_channel_pay_restart_close_withdraw(node_factory, bitcoind):
    a, b = node_factory.line_graph(2, opts={'may_reconnect': True})
    channel = only_one(a.rpc.listpeerchannels()['channels'])
    assert 'unified_sigs/even' in channel['channel_type']['names']
    assert_unified_witnesses(bitcoind, channel['funding_txid'], 1)
    a.pay(b, 1000000)
    # Reopening the DB must not revert the negotiated signing type.
    a.restart()
    wait_for(lambda: only_one(a.rpc.listpeerchannels()['channels'])['peer_connected'])
    a.pay(b, 1000000)
    a.rpc.close(b.info['id'])
    wait_for(lambda: len(bitcoind.rpc.getrawmempool()) == 1)
    txid = only_one(bitcoind.rpc.getrawmempool())
    assert_unified_witnesses(bitcoind, txid, 2)
    bitcoind.generate_block(6)
    sync_blockheight(bitcoind, [a, b])
    wait_for(lambda: any(o['status'] == 'confirmed' for o in a.rpc.listfunds()['outputs']))
    result = a.rpc.withdraw(bitcoind.rpc.getnewaddress(), 10000)
    assert_unified_witnesses(bitcoind, result['txid'], 1)
    bitcoind.generate_block(1)
    assert bitcoind.rpc.getrawtransaction(result['txid'], True)['confirmations'] >= 1

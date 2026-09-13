"""Isolated regtest policy tests; no production chain or funds involved."""
from fixtures import *  # noqa: F401,F403
from pyln.client import RpcError
import pytest


def test_blake2b_migration_accepts_legacy(node_factory):
    blake, legacy = node_factory.get_nodes(2, opts=[{}, {'dev-force-features': '-4110'}])
    blake.rpc.connect(legacy.info['id'], 'localhost', legacy.port)
    assert blake.rpc.listpeers()['peers'][0]['connected']
    # Required invoice markers prevent ordinary cross-network payment reads.
    inv = blake.rpc.invoice(1000, 'blake', 'proposal test')['bolt11']
    assert len(inv) < 2000  # Field fits without truncation despite placeholder bit.
    assert blake.rpc.decodepay(inv)['payment_hash']
    with pytest.raises(RpcError, match='feature'):
        legacy.rpc.decodepay(inv)
    legacy_inv = legacy.rpc.invoice(1000, 'legacy', 'proposal test')['bolt11']
    with pytest.raises(RpcError, match='feature'):
        blake.rpc.decodepay(legacy_inv)
    offer = blake.rpc.offer('1000msat', 'proposal offer')['bolt12']
    # Fetch validates an offer before attempting any payment or routing.
    with pytest.raises(RpcError, match='feature'):
        legacy.rpc.fetchinvoice(offer)


@pytest.mark.parametrize('incoming', [False, True])
def test_blake2b_strict_rejects_legacy(node_factory, incoming):
    blake, legacy = node_factory.get_nodes(2, opts=[
        {'blake2b-peer-policy': 'strict', 'may_reconnect': True},
        {'dev-force-features': '-4110', 'may_reconnect': True}])
    source, target = (legacy, blake) if incoming else (blake, legacy)
    with pytest.raises(RpcError):
        source.rpc.connect(target.info['id'], 'localhost', target.port)
    assert not any(p['connected'] for p in blake.rpc.listpeers()['peers'])


def test_blake2b_strict_accepts_blake_and_reconnects(node_factory):
    a, b = node_factory.get_nodes(2, opts={'blake2b-peer-policy': 'strict'})
    a.rpc.connect(b.info['id'], 'localhost', b.port)
    assert a.rpc.listpeers()['peers'][0]['connected']
    a.rpc.disconnect(b.info['id'])
    b.rpc.connect(a.info['id'], 'localhost', a.port)
    assert b.rpc.listpeers()['peers'][0]['connected']
    offer = a.rpc.offer('1000msat', 'compatible offer')['bolt12']
    invoice = b.rpc.fetchinvoice(offer)['invoice']
    assert b.rpc.decode(invoice)['valid']

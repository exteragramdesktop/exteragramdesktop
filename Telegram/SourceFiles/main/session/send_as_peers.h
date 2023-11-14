/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class PeerData;

namespace Main {

class Session;

struct SendAsPeer {
	not_null<PeerData*> peer;
	bool premiumRequired = false;

	friend inline auto operator<=>(SendAsPeer, SendAsPeer) = default;
};

class SendAsPeers final {
public:
	explicit SendAsPeers(not_null<Session*> session);

	bool shouldChoose(not_null<PeerData*> peer);
	void refresh(not_null<PeerData*> peer, bool force = false);
	[[nodiscard]] const std::vector<SendAsPeer> &list(
		not_null<PeerData*> peer) const;
	[[nodiscard]] rpl::producer<not_null<PeerData*>> updated() const;

	void saveChosen(not_null<PeerData*> peer, not_null<PeerData*> chosen);
	void setChosen(not_null<PeerData*> peer, PeerId chosenId);
	[[nodiscard]] PeerId chosen(not_null<PeerData*> peer) const;

	// If !list(peer).empty() then the result will be from that list.
	[[nodiscard]] not_null<PeerData*> resolveChosen(
		not_null<PeerData*> peer) const;

	[[nodiscard]] static not_null<PeerData*> ResolveChosen(
		not_null<PeerData*> peer,
		const std::vector<SendAsPeer> &list,
		PeerId chosen);

private:
	void request(not_null<PeerData*> peer);

	const not_null<Session*> _session;
	const std::vector<SendAsPeer> _onlyMe;

	base::flat_map<not_null<PeerData*>, std::vector<SendAsPeer>> _lists;
	base::flat_map<not_null<PeerData*>, crl::time> _lastRequestTime;
	base::flat_map<not_null<PeerData*>, PeerId> _chosen;

	rpl::event_stream<not_null<PeerData*>> _updates;

	rpl::lifetime _lifetime;

};

} // namespace Main

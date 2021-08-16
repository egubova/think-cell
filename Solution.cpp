#include <map>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K, V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
		: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		if (!(keyBegin < keyEnd)) {
			return;
		}

		auto it = m_map.lower_bound(keyBegin);
		if (it == m_map.end()) {
			m_map.insert(std::make_pair(keyBegin, val));
			m_map.insert(std::make_pair(keyEnd, m_valBegin));
		}
		else {
			std::map<K, bool> toRemove;
			if (it != m_map.begin()) {
				--it;
			}
			while (it != m_map.end() && it->first < keyEnd) {
				if (keyBegin < it->first) {
					toRemove[it->first];
				}
				if (!(keyBegin < it->first) && !(it->first < keyBegin)) {
					toRemove[it->first];
				}
				if (++it != m_map.end() && keyEnd < it->first) {
					m_map.insert(std::make_pair(keyEnd, (--it)->second));
					break;
				}
			}
			for (auto [keyToRemove, trash] : toRemove) {
				m_map.erase(keyToRemove);
			}
			if (!m_map.empty() && keyEnd < m_map.begin()->first) {
				m_map.insert(std::make_pair(keyEnd, m_valBegin));
			}
			m_map.insert(std::make_pair(keyBegin, val));
		}
		it = m_map.find(keyBegin);
		std::map<K, bool> keysToRemove;
		auto newKeyBegin = keyBegin;
		while (it != m_map.begin()) {
			--it;
			if (!(it->second == val)) {
				break;
			}
			keysToRemove[newKeyBegin];
			newKeyBegin = it->first;
		}
		for (const auto& [key, trash] : keysToRemove) {
			m_map.erase(key);
		}
		it = m_map.find(newKeyBegin);
		it++;
		keysToRemove.clear();
		while (it != m_map.end()) {
			if (!(it->second == val)) {
				break;
			}
			auto key = it->first;
			keysToRemove[key];
			++it;
		}
		for (const auto& [key, trash] : keysToRemove) {
			m_map.erase(key);
		}

		if (m_map.rbegin()->second != m_valBegin) {
			m_map.insert(std::make_pair(keyEnd, m_valBegin));
		}
		if (m_map.begin()->second == m_valBegin) {
			m_map.erase(m_map.begin());
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}
};
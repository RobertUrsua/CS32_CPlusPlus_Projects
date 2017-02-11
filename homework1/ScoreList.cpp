#include "ScoreList.h"
#include "Sequence.h"

ScoreList::ScoreList() {
}

bool ScoreList::add(unsigned long score) {
	if (score <= 100) {
		if (m_scores.insert(score) != -1)
			return true;
		else
			return false;
	}
	return false;
}

bool ScoreList::remove(unsigned long score) {
	if (score <= 100) {
		int scorePos = m_scores.find(score);

		if (scorePos != -1) {
			m_scores.erase(scorePos);
			return true;
		}
		else
			return false;
	}
	return false;
}

int ScoreList::size() const {
	return m_scores.size();
}

unsigned long ScoreList::minimum() const {
	if (m_scores.size()) {
		unsigned long temp;
		m_scores.get(0, temp);
		return temp;
	}
	return NO_SCORE;
}

unsigned long ScoreList::maximum() const {
	if (m_scores.size()) {
		unsigned long temp;
		m_scores.get(m_scores.size()-1, temp);
		return temp;
	}
	return NO_SCORE;

}

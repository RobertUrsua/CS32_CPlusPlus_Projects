#include "Sequence.h"

////////////////////////////////////////
// Class Sequence Implementation
////////////////////////////////////////

Sequence::Sequence() : m_size(0) {
}

bool Sequence::empty() const {
	return m_size ? false : true;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (m_size == DEFAULT_MAX_ITEMS || pos < 0 || pos > m_size) 
		return false;

	int i;
	for (i = m_size; i > pos; i--) {
		if (i) {
			m_seqArray[i] = m_seqArray[i - 1];
		}
	}

	m_seqArray[pos] = value;
	m_size++;
	return true;
}

int Sequence::insert(const ItemType& value) {
	int i;
	for (i = 0; i < m_size; i++) {
		if (value <= m_seqArray[i]) {
			if (insert(i, value))
				return i;
			else
				return -1;
		}
	}
	if (insert(m_size, value))
		return m_size - 1;
		// if insert was succesful, m_size is incremented by one so the element is actuall in m_size-1;
	else
		return -1;

	// no need for m_size++ in this function since the other insert implements it already
}

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= m_size)
		return false;
	int i = pos;

	for (i = pos; i < m_size - 1; i++) {
		m_seqArray[i] = m_seqArray[i + 1];
	}
	m_size--;
	return true;
}

int Sequence::remove(const ItemType& value) {
	int curNum = 0;
	int nextNum = 0;

	while(curNum + nextNum < m_size) {
		if (m_seqArray[curNum+nextNum] == value) {
			// essentially skips the element that's to be removed so it isn't copied
			nextNum++;
		}
		else {
			m_seqArray[curNum] = m_seqArray[curNum + nextNum];
			curNum++;
		}
	}
	m_size -= nextNum;

	return nextNum;
}

bool Sequence::get(int pos, ItemType& value)  const {
	if (pos >= 0 && pos < m_size) {
		value = m_seqArray[pos];;
		return true;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos >= 0 && pos < m_size) {
		m_seqArray[pos] = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const {
	int pos = 0;
	for (pos = 0; pos < m_size; pos++) {
		if (value == m_seqArray[pos])
			return pos;
	}
	return -1;
}

void Sequence::swap(Sequence& other) {
	int pos = 0;
	ItemType temp;

	for (pos = 0; pos < other.m_size || pos < m_size; pos++) {
		temp = other.m_seqArray[pos];
		other.m_seqArray[pos] = m_seqArray[pos];
		m_seqArray[pos] = temp;
	}
	
	int tempSize = other.m_size;
	other.m_size = m_size;
	m_size = tempSize;
}

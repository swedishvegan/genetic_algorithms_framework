#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <cstdlib>
#include <vector>

#define uint unsigned int

class Permutation {
public:

	static bool randomInit;

	Permutation(uint size);

	template <typename Data>
	Permutation(Data data, uint size);

	uint size() const;

	uint& operator [] (uint idx);

	void swap(uint idx1, uint idx2);

	void scramble(uint startIdx, uint endIdx);

#ifdef ORDER_CROSSOVER
	Permutation orderCrossover(Permutation& p);
#endif

#ifdef EDGE_CROSSOVER
	Permutation edgeCrossover(Permutation& p);
#endif

protected:

	std::vector<uint> perm;

#ifdef EDGE_CROSSOVER

	struct EdgeList { uint e1, e2; };
	std::vector<EdgeList> edges;

	void updateEdges();

	void removeReferences(uint edge, Permutation& p);

	uint numDistinctEdges(uint idx, Permutation& p);

	uint getCommonEdge(uint idx, Permutation& p);

#endif

};

bool Permutation::randomInit = true;

Permutation::Permutation(uint size) {

	perm.resize(size);
#ifdef EDGE_CROSSOVER
	edges.resize(size);
#endif

	if (!randomInit) return;

	for (uint i = 0u; i < size; i++) {

		uint curEdge = (rand() % (size - i)) + 1u;
		uint idx = 0u;

		for (uint j = 0u; j < size; j++) {

			bool filled = false;
			for (uint k = 0u; k < i; k++) if (j == perm[k]) { filled = true; break; }

			if (!filled) idx++;
			if (idx == curEdge) { curEdge = j; break; }

		}

		perm[i] = curEdge;

	}

}

template <typename Data>
Permutation::Permutation(Data data, uint size) {

	perm.resize(size);
#ifdef EDGE_CROSSOVER
	edges.resize(size);
#endif
	
	for (uint i = 0u; i < size; i++) perm[i] = data[i];

}

uint Permutation::size() const { return perm.size(); }

uint& Permutation::operator [] (uint idx) { return perm[idx % size()]; }

void Permutation::swap(uint idx1, uint idx2) {

	uint temp = perm[idx1];
	perm[idx1] = perm[idx2];
	perm[idx2] = temp;

}

void Permutation::scramble(uint startIdx, uint endIdx) {

	if (startIdx >= size() || endIdx >= size() || startIdx >= endIdx) return;

	uint temp = perm[startIdx];
	for (uint i = startIdx; i < endIdx; i++) perm[i] = perm[i + 1u];
	perm[endIdx] = temp;

}

#ifdef ORDER_CROSSOVER
Permutation Permutation::orderCrossover(Permutation& p) {

	bool tempInit = randomInit;
	randomInit = false;

	Permutation p_out(size());

	uint startIdx = rand() % (size() - 1u);
	uint endIdx = startIdx + (rand() % (size() - startIdx));
	
	for (uint i = 0u; i < size(); i++) {

		if (i >= startIdx && i <= endIdx) p_out[i] = perm[i];
		else p_out[i] = size();

	}

	uint pIdx = endIdx + 1u;
	if (pIdx == size()) pIdx = 0u;

	for (uint i = 1u; i < size() - (endIdx - startIdx); i++) {
		
		uint ii = endIdx + i;
		if (ii >= size()) ii -= size();

		if (p_out[ii] < size()) continue;

		bool isDuplicate = true;
		while (isDuplicate) {

			isDuplicate = false;
			for (uint j = 0u; j < size(); j++) if (p[pIdx] == p_out[j]) { isDuplicate = true; break; }

			if (isDuplicate) pIdx++;
			if (pIdx == size()) pIdx = 0u;

		}
		
		p_out[ii] = p[pIdx];

	}

	randomInit = tempInit;
	return p_out;

}
#endif

#ifdef EDGE_CROSSOVER
Permutation Permutation::edgeCrossover(Permutation& p) {

	bool tempInit = randomInit;
	randomInit = false;

	updateEdges();
	p.updateEdges();

	Permutation p_out(size());
	p_out[0] = rand() % size();
	uint curEdge = p_out[0];

	for (uint i = 1u; i < size(); i++) {

		removeReferences(curEdge, p);

		uint commonEdge = getCommonEdge(curEdge, p);
		if (commonEdge < size()) {

			p_out[i] = commonEdge;
			curEdge = commonEdge;
			continue;

		}

#define _Permutation_checkEdgeCandidate(edge) \
\
isRepeat = false; for (uint j = 0u; j < numCandidates; j++) if (edgeCandidates[j] == edge) { isRepeat = true; break; } \
if (!isRepeat && edge < size()) { edgeCandidates[numCandidates] = edge; numCandidates++; }

#define _Permutation_getEdgeCandidates(curEdge) \
\
static uint edgeCandidates[4]; \
edgeCandidates[0] = edges[curEdge].e1; \
uint numCandidates = (edgeCandidates[0] < size()) ? 1u : 0u; \
bool isRepeat; \
\
_Permutation_checkEdgeCandidate(edges[curEdge].e2); \
_Permutation_checkEdgeCandidate(p.edges[curEdge].e1); \
_Permutation_checkEdgeCandidate(p.edges[curEdge].e2);

		_Permutation_getEdgeCandidates(curEdge);

		if (numCandidates == 0u) {

			curEdge = (rand() % (size() - i)) + 1u;
			uint idx = 0u;

			for (uint j = 0u; j < size(); j++) {

				bool filled = false;
				for (uint k = 0u; k < i; k++) if (j == p_out[k]) { filled = true; break; }

				if (!filled) idx++;
				if (idx == curEdge) { curEdge = j; break; }

			}

			p_out[i] = curEdge;
			continue;

		}
		
		uint shortestList = numDistinctEdges(edgeCandidates[0], p);
		uint shortestListIdx = 0u;

		for (uint j = 1u; j < numCandidates; j++) {

			uint numDistEdges = numDistinctEdges(edgeCandidates[j], p);
			if (numDistEdges < shortestList) {

				shortestList = numDistEdges;
				shortestListIdx = j;

			}

		}
		
		curEdge = edgeCandidates[shortestListIdx];
		p_out[i] = curEdge;

	}

	randomInit = tempInit;
	return p_out;

}

void Permutation::updateEdges() {

	int isize = (int)size();

	for (int i = 0; i < isize; i++) {

		int ii;
		for (int j = 0; j < isize; j++) if (perm[j] == (uint)i) ii = j;

		int i1 = ii - 1; if (i1 < 0) i1 = isize - 1;
		int i2 = ii + 1; if (i2 >= isize) i2 = 0;

		edges[i].e1 = perm[i1];
		edges[i].e2 = perm[i2];

	}

}

void Permutation::removeReferences(uint edge, Permutation& p) {

	for (uint i = 0u; i < size(); i++) {

		if (edges[i].e1 == edge) edges[i].e1 = size();
		if (edges[i].e2 == edge) edges[i].e2 = size();
		if (p.edges[i].e1 == edge) p.edges[i].e1 = size();
		if (p.edges[i].e2 == edge) p.edges[i].e2 = size();

	}

}

uint Permutation::numDistinctEdges(uint idx, Permutation& p) {
	
	_Permutation_getEdgeCandidates(idx);
	return numCandidates;

}

uint Permutation::getCommonEdge(uint idx, Permutation& p) {

#define _Permutation_testCommonEdge(edge1, edge2) if (edge1 == edge2 && edge1 < size()) return edge1;

	_Permutation_testCommonEdge(edges[idx].e1, edges[idx].e2);
	_Permutation_testCommonEdge(edges[idx].e1, p.edges[idx].e1);
	_Permutation_testCommonEdge(edges[idx].e1, p.edges[idx].e2);
	_Permutation_testCommonEdge(edges[idx].e2, p.edges[idx].e1);
	_Permutation_testCommonEdge(edges[idx].e2, p.edges[idx].e2);
	_Permutation_testCommonEdge(p.edges[idx].e1, p.edges[idx].e2);

	return size();

}
#endif

#endif
/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include "segment_graph.h"


bool operator<(const edge &a, const edge &b)
{
	return a.w < b.w;
}

/*
* Segment a graph
*
* Returns a disjoint-set forest representing the segmentation.
*
* num_vertices: number of vertices in graph.
* num_edges: number of edges in graph
* edges: array of edges.
* fl_con: constant for treshold function.
*/
Universe *segment_graph(int num_vertices, int num_edges, edge *edges, float fl_con)
{
	// sort edges by weight
	std::sort(edges, edges + num_edges);

	// make a disjoint-set forest
	Universe *po_uni = new Universe(num_vertices);

	// init thresholds
	float *pafl_threshold = new float[num_vertices];

	for (int i = 0; i < num_vertices; i++) {
		pafl_threshold[i] = THRESHOLD(1, fl_con);
	}

	// for each edge, in non-decreasing weight order...
	for (int i = 0; i < num_edges; i++) {
		edge *po_edge = &edges[i];
		// components conected by this edge
		int i_a = po_uni->find(po_edge->a);
		int i_b = po_uni->find(po_edge->b);

		if (i_a != i_b) {
			if ((po_edge->w <= pafl_threshold[i_a]) && (po_edge->w <= pafl_threshold[i_b])) {
				po_uni->join(i_a, i_b);
				i_a = po_uni->find(i_a);
				pafl_threshold[i_a] = po_edge->w + THRESHOLD(po_uni->size(i_a), fl_con);
			}
		}
	}

	// free
	delete pafl_threshold;
	return po_uni;
}
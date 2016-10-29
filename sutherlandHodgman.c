// From Foley et al., Introduction to Computer Graphics, Prog 3.9

typedef struct vertex {
	float x, y;
} vertex;

typedef vertex edge[2];
typedef vertex vertexArray[MAX];	/* MAX is a declared constant */

void Intersect(vertex first, vertex second, vertex *clipBoundary,
			  vertex *intersectPt)
{
	if (clipBoundary[0].y == clipBoundary[1].y) {		/*horizontal*/
		intersectPt->y = clipBoundary[0].y;
		intersectPt->x = first.x + (clipBoundary[0].y - first.y) *
			(second.x - first.x) / (second.y - first.y);
	} else {											/*vertical*/
		intersectPt->x = clipBoundary[0].x;
		intersectPt->y = first.y + (clipBoundary[0].x - first.x) *
			(second.y - first.y) / (second.x - first.x);
	}
}

boolean Inside(vertex testVertex, vertex *clipBoundary)
{
	if (clipBoundary[1].x > clipBoundary[0].x)	/*bottom*/
		if (testVertex.y >= clipBoundary[0].y)
			return TRUE;
	if (clipBoundary[1].x < clipBoundary[0].x)	/*top*/
		if (testVertex.y <= clipBoundary[0].y)
			return TRUE;
	if (clipBoundary[1].y > clipBoundary[0].y)	/*right*/
		if (testVertex.x <= clipBoundary[1].x)
			return TRUE;
	if (clipBoundary[1].y < clipBoundary[0].y)	/*left*/
		if (testVertex.x >= clipBoundary[1].x)
			return TRUE;
	return FALSE;
}

void Output(vertex newVertex, int *outLength, vertex *outVertexArray)
{
	(*outLength)++;
	outVertexArray[*outLength - 1].x = newVertex.x;
	outVertexArray[*outLength - 1].y = newVertex.y;
}

void SutherlandHodgmanPolygonClip(vertex *inVertexArray,
	vertex *outVertexArray, int inLength, int *outLength, vertex *clip_boundary)
{
	vertex s, p, i;
	int j;

	*outLength = 0;
	s = inVertexArray[inLength - 1];	/* Start with the last vertex in inVertexArray */
	for (j = 0; j < inLength; j++) {
		p = inVertexArray[j];	/* Now s and p correspond to the vertices in Fig.3.33 */
		if (Inside(p, clip_boundary)) {	/* Cases 1 and 4 */
			if (Inside(s, clip_boundary))
				Output(p, outLength, outVertexArray);	/* Case 1 */
			else {										/* Case 4 */
				Intersect(s, p, clip_boundary, &i);
				Output(i, outLength, outVertexArray);
				Output(p, outLength, outVertexArray);
			}
		} else if(Inside(s, clip_boundary)) {			/* Cases 2 and 3 */
			Intersect(s, p, clip_boundary, &i);			/* Case 2 */
			Output(i, outLength, outVertexArray);
		}												/* No action for case 3 */
		s = p;					/* Advance to next pair of vertices */
	}
}

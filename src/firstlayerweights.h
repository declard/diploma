#ifndef FIRSTLAYERWEIGHTS_H
#define FIRSTLAYERWEIGHTS_H
static const double w=-2.0,q=4.0*w;
static double firstLayerData[][3][3]={
/*	w,1,w,
	w,1,w,
	w,1,w,*/

	w,3,w,
	w,3,w,
	w,w,3,

	w,3,w,
	w,3,w,
	3,w,w,

	w,w,3,
	w,3,w,
	w,3,w,

	3,w,w,
	w,3,w,
	w,3,w,

	w,3,w,
	3,w,w,
	w,3,w,

	w,3,w,
	w,w,3,
	w,3,w,

	w,w,w,
	3,3,w,
	w,3,w,

	w,w,w,
	w,3,3,
	w,3,w,

	w,3,w,
	3,3,w,
	w,w,w,

	w,3,w,
	w,3,3,
	w,w,w,

/*	w,w,w,
	1,1,1,
	w,w,w,*/

	w,w,w,
	w,3,3,
	3,w,w,

	w,w,w,
	3,3,w,
	w,w,3,

	3,w,w,
	w,3,3,
	w,w,w,

	w,w,3,
	3,3,w,
	w,w,w,

	w,w,w,
	w,3,w,
	w,3,3,

	w,w,w,
	w,3,w,
	3,3,w,

	w,w,w,
	w,3,3,
	w,w,3,

	w,w,3,
	w,3,3,
	w,w,w,

	w,3,3,
	w,3,w,
	w,w,w,

	3,3,w,
	w,3,w,
	w,w,w,

	3,w,w,
	3,3,w,
	w,w,w,

	w,w,w,
	3,3,w,
	3,w,w,

	w,q,q,
	6,6,q,
	w,q,q,

	w,6,w,
	q,6,q,
	q,q,q,

	q,q,w,
	q,6,6,
	q,q,w,

	q,q,q,
	q,6,q,
	w,6,w,

	q,q,q,
	w,6,q,
	6,w,q,

	q,q,q,
	q,6,w,
	q,q,6,

	q,w,6,
	q,6,w,
	q,q,q,

	6,w,q,
	w,6,q,
	q,q,q

/*	w,w,1,
	w,1,w,
	1,w,w,

	1,w,w,
	w,1,w,
	w,w,1*/
};
#endif // FIRSTLAYERWEIGHTS_H

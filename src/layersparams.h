#ifndef LAYERSPARAMS_H
#define LAYERSPARAMS_H

static int		N0		= 65;
static int		Ng		= 71;
static int		Nc[]	= {37,		21,			13,			1};
static int		Ns[]	= {68,		38,			22,			5};
static int		mc[]	= { 2,		2,			2,			1};
static double	Ag		= 3.3;
static double	Ago		= 1.5;
static double	As[]	= {3.4,		3.4,		3.4,		4.9};
static double	Aco[]	= {3.4,		3.4,		3.4,		7.0};
static double	Ac[]	= {5.4,		4.8,		Aco[2],		7.0};
static double	D[]		= {0.0,		3.1,		3.1,		4.5};
static double	Asini[]	= {0.0,		As[1],		As[2],		As[3]};
static double	Al[]	= {0.0,		D[1],		D[2],		D[3]};
static double	gamcg	= 0.05;
static double	gamS[]	= {0.7,		0.7,		0.7,		0.6};
static double	coS[]	= {1.0,		1.0,		1.0,		1.0};
static double	gamSini[]={0.0,		gamS[1],	gamS[2],	gamS[3]};
static double	coSini[]= {0.0001,	0.0001,		0.0001,		0.0001};
static double	gamC[]	= {0.7,		0.7,		0.7,		1.0};
static double	gamCo[]	= {0.12,	0.12,		0.12,		1.0};
static double	gamCp[]	= {0.8,		0.8,		0.8,		1.0};
static double	coC[]	= {4.0,		4.0,		4.0,		1.0};
static double	pnC[]	= {1.0,		1.0,		1.0,		1.0};
static double	gaml[]	= {0.0,		0.7,		0.7,		1.0};
static double	col[]	= {0.0,		1.0,		1.0,		1.0};
static double	thetal[]= {0.55,	0.66,		0.67,		0.30};
static double	thetar[]= {0.55,	0.51,		0.58,		0.30};
static double	qs[]	= {100.0,	100.0,		100.0,		100.0};
static double	thrini[]= {0.0,		0.3,		0.3,		0.3};
//static int		KMax[]	= {16,		50,			40,			20};
//static int		KMax[]	= {16,		200,		500,		300}; // Original Fukushima's
static int		KMax[]	= {16,		60,			70,			40};
static int		K[]		= {16,		0,			0,			0};
static int		repeat[]= {1,		1,			1,			1};


enum	{	unsupervised,	supervised,	lastUnsupervised,	none};
static int		trainType[]={
	none,
	unsupervised,
	unsupervised,
	lastUnsupervised
};

static int layersCount=4;
static int learnFromLayer=0;
static int learnToLayer=layersCount;

#endif // LAYERSPARAMS_H

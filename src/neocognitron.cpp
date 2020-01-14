#include "neocognitron.h"
#include <QDir>

Neocognitron::Neocognitron(ParametersContainer*p) {
	this->p=p;
	trained=0;
	init();
	//addNew("new",N0,"1");
}

Layer::Data Neocognitron::activate(Layer::Data data,int count) {
	if (p->useGLayer) data=glayer->activate(data);
	for(int i=0;i<count;i++) {
		data=layers.at(i)->activate(data);
	}
	return data;
}

void Neocognitron::addNew(QString name,int size,QString cat) {
	patterns+=Pattern(name,Matrix<char>(MatrixIndex(size),0),cat,true,true);
}

QImage Neocognitron::toImage(MatrixC m) {
	QImage res(m.size(),QImage::Format_RGB16);
	MatrixIndex i,f,t(m.size());
	for2DIncr(i,f,t) {
		char v(m.at(i));
		res.setPixel(i,qRgb(v,v,v));
	}
	return res;
}

MatrixC Neocognitron::fromImage(const QImage&im) {
	MatrixC res(im.size());
	MatrixIndex i,f,t(im.size());
	for2DIncr(i,f,t) {
		QRgb v(im.pixel(i));
		int c=qRed(v);
		c+=qGreen(v);
		c+=qBlue(v);
		res[i]=c/3;
	}
	return res;
}

static Neocognitron::Pattern addPat(QString path,QString file) {
	QImage im;
	MatrixC m;

	static int i=0;
	im.load(path+'/'+file,"BMP");
	QString cat=file.left(1);
	m=Neocognitron::fromImage(im);
	return Neocognitron::Pattern(file,MatrixC(m),cat,/*i++%2*/true,true);
}

static Neocognitron::Pattern addPat(char data[28][28],QString klass,bool l,bool t,QString name) {
	MatrixC mc((char*)data,MatrixIndex(28,28));
	QImage im(Neocognitron::toImage(mc));
	im=im.scaled(65,65,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	return Neocognitron::Pattern(name,Neocognitron::fromImage(im),klass,/*i++%2*/l,t);
}

void Neocognitron::loadMNISTPatterns(QString dir) {
	QFile trd(dir+"train-images.idx3-ubyte"),
		trl(dir+"train-labels.idx1-ubyte"),
		ted(dir+"t10k-images.idx3-ubyte"),
		tel(dir+"t10k-labels.idx1-ubyte");
	trd.open(QIODevice::ReadOnly);
	trl.open(QIODevice::ReadOnly);
	ted.open(QIODevice::ReadOnly);
	tel.open(QIODevice::ReadOnly);
	trd.seek(16); trl.seek(8);
	ted.seek(16); tel.seek(8);
	QStringList klasses;
	for(int i=0;i<10;i++)
		klasses+=QString::number(i);
	char pattern[28][28];
	char label;
	QString trformat("train%1");
	for(int i=0;i<500;i++) {
		trd.read((char*)pattern,28*28);
		trl.read(&label,1);
		//if (label==0||label==1||label==2||label==3||label==4)
			patterns+=addPat(pattern,klasses.at(label),true,false,trformat.arg(i,3,10,QChar('0')));
		//else i--;
	}
	QString teformat("test%1");
	for(int i=0;i<500;i++) {
		ted.read((char*)pattern,28*28);
		tel.read(&label,1);
		//if (label==0||label==1||label==2||label==3||label==4)
			patterns+=addPat(pattern,klasses.at(label),false,true,teformat.arg(i,3,10,QChar('0')));
		//else i--;
	}
}

void Neocognitron::init() {
	/*QDir dir("../in/");
	QStringList files=dir.entryList(QDir::Files);
	for(int i=0;i<files.size();i++)
		patterns+=addPat(dir.path(),files.at(i));*/

	loadMNISTPatterns("../in/");

	glayer=new GLayer();
	glayer->Ng=MatrixIndex(p->Ng);
	int Ag=BasicLayer::calcN(p->Ag,p->N0,p->Ng);
	glayer->Ag=MatrixIndex(Ag);
	glayer->cg=GLayer::makeCg(Ag,p->Ag,p->Ago,p->gamcg);
	for(int i=0;i<p->layersCount();i++) {
		addLayer(i);
	}
	layers[0]->slayer->setKin(2);
	layers[1]->slayer->setKin(16);
	layers[2]->slayer->setKin(1);
	layers[3]->slayer->setKin(1);
}

//static int Ksbo[] = {0,1,1,0};
//static int Kcbo[] = {3,2,0,3};
static int Ksbo[] = {0,1,1,0};
static int Kcbo[] = {4,2,0,2};

void Neocognitron::addLayer(int i) {
	int NcPrev;
	if (i) NcPrev=p->at(i-1).Nc;
	else NcPrev=(p->useGLayer?p->Ng:p->N0);
	int A=BasicLayer::calcN(p->at(i).As,NcPrev,p->at(i).Ns);

	int S=p->at(i).Ns;
	MatrixD cs=SLayer::makeC(A,p->at(i).As,p->at(i).gamS,p->at(i).coS);
	SLayer*s=new SLayer(
			MatrixIndex(A),
			MatrixIndex(S),
			p->at(i).thetar/(1-p->at(i).thetar),
			cs);
	s->cs=cs;
	s->setKout(p->at(i).K);
	s->setThreadCount(p->threads);
	s->explicitBorderOffset=MatrixIndex(Ksbo[i]);

	CLayer*cl;
	if (i<p->layersCount()-1) {
		int D;
		if (p->at(i).mc==2&&p->at(i).Ns%2==1)
			D=((int)p->at(i).Ac)*2+1;
		else D=((int)(p->at(i).Ac+0.5))*2;

		int C=p->at(i).Nc;
		int factor=p->at(i).mc;

		MiddleCLayer*c=new MiddleCLayer(MatrixIndex(D),MatrixIndex(C),MatrixIndex(factor));

		c->initDs(
				D,
				p->at(i).Ac,
				p->at(i).Aco,
				p->at(i).gamC,
				p->at(i).gamCo,
				p->at(i).gamCp,
				p->at(i).coC,
				p->at(i).pnC);
		c->explicitBorderOffset=MatrixIndex(Kcbo[i]);
		cl=c;
	}
	else cl=new LastCLayer();
	cl->setThreadCount(p->threads);
	layers+=new Layer(s,cl);
}

QDataStream&Neocognitron::save(QDataStream&s) const {
	s<<patterns;
	s<<trained;
	for(int i=0;i<trained;i++) {
		SLayer*sl=layers.at(i)->slayer;
		s<<sl->Kin();
		s<<sl->Kout();
		for(int ii=0;ii<sl->Kout();ii++) {
			const SLayer::Plane&p=sl->planes[ii];
			s<<p.a;
			s<<p.b;
		}
	}
	if (trained==layers.size())
		s<<klassMap;
	return s;
}

void Neocognitron::load(QDataStream&s) {
	int ti,lc;
	patterns.clear();
	s>>patterns;
	s>>lc;
	Q_ASSERT(lc<=layers.size());
	trained=lc;
	int i;
	for(i=0;i<lc;i++) {
		SLayer*sl=layers[i]->slayer;
		s>>ti;
		sl->setKin(ti);
		s>>ti;
		sl->setKout(ti);
		for(int ii=0;ii<sl->Kout();ii++) {
			SLayer::Plane&p=sl->planes[ii];
			s>>p.a;
			s>>p.b;
		}
	}
	if (lc<layers.size())
		layers[i]->slayer->setKin(ti);
	else s>>klassMap;
}

QDataStream&operator<<(QDataStream&s,const Neocognitron::Pattern&p) {
	s<<p.name<<p.data<<p.klass<<p.train<<p.test;
	return s;
}


QDataStream&operator>>(QDataStream&s,Neocognitron::Pattern&p) {
	s>>p.name>>p.data>>p.klass>>p.train>>p.test;
	return s;
}

QPair< QString,QVector<double> > Neocognitron::getClass(Data data) const {
	QVector<double> weights(data.size());
	for(int ii=0;ii<data.size();ii++)
		weights[ii]=data.at(ii).at(MatrixIndex(0,0));
	double wmax=0.0;
	int kmax=-1;
	for(int ii=0;ii<weights.size();ii++) {
		if (wmax<weights[ii]) {
			kmax=ii;
			wmax=weights[ii];
		}
	}
	QString klass;
	if (kmax!=-1) klass=klassMap.at(kmax);
	return QPair< QString,QVector<double> >(klass,weights);
}

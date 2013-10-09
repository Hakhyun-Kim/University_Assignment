// MeshlessDeforVation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Jfl/Fl_3D_View.h"

void print( const char name[], matrix& A )
{
	cout<<name<<endl;
	cout<<A[0][0]<<" "<<A[0][1]<<" "<<A[0][2]<<endl;
	cout<<A[1][0]<<" "<<A[1][1]<<" "<<A[1][2]<<endl;
	cout<<A[2][0]<<" "<<A[2][1]<<" "<<A[2][2]<<endl;
	cout<<endl;
}

void printOrtho( const char name[], matrix& R )
{
	cout<<name<<endl;
	cout<<R[0]%R[0]<<" "<<R[0]%R[1]<<" "<<R[0]%R[2]<<endl;
	cout<<R[1]%R[0]<<" "<<R[1]%R[1]<<" "<<R[1]%R[2]<<endl;
	cout<<R[2]%R[0]<<" "<<R[2]%R[1]<<" "<<R[2]%R[2]<<endl;
	cout<<endl;
}

//qudaratic 에 필요할듯?? jacobi??
void orthogonalize( const matrix& in, matrix& out )
{
	static matrixN m(3,3), V(3,3);
	static vectorN d(3);
	static matrix U, Vt;

	m[0][0]=in.getValue( 0, 0 );
	m[0][1]=in.getValue( 0, 1 );
	m[0][2]=in.getValue( 0, 2 );
	m[1][0]=in.getValue( 1, 0 );
	m[1][1]=in.getValue( 1, 1 );
	m[1][2]=in.getValue( 1, 2 );
	m[2][0]=in.getValue( 2, 0 );
	m[2][1]=in.getValue( 2, 1 );
	m[2][2]=in.getValue( 2, 2 );

	m.SVdecompose( d, V );
	U =matrix( m[0][0],m[0][1],m[0][2], m[1][0],m[1][1],m[1][2], m[2][0],m[2][1],m[2][2] );
	Vt=matrix( V[0][0],V[1][0],V[2][0], V[0][1],V[1][1],V[2][1], V[0][2],V[1][2],V[2][2] );
	if( d[0] < 0 ) { Vt[0][0]=-Vt[0][0], Vt[0][1]=-Vt[0][1], Vt[0][2]=-Vt[0][2]; }
	if( d[1] < 0 ) { Vt[1][0]=-Vt[1][0], Vt[1][1]=-Vt[1][1], Vt[1][2]=-Vt[1][2]; }
	if( d[2] < 0 ) { Vt[2][0]=-Vt[2][0], Vt[2][1]=-Vt[2][1], Vt[2][2]=-Vt[2][2]; }
	out = U*Vt;
	// quater q = Matrix2Quater( out ).normalize();
	//cout<<"q: "<<q[0]<<" "<<q[1]<<" "<<q[2]<<" "<<q[3]<<endl;
	// out = Quater2Matrix( q );
}

//qudaratic 에 필요할듯?? jacobi??
void orthogonalizeN( const matrixN& in, matrixN& out )
{
	int row = in.row();
	int col = in.column();
	static matrixN m(row,col), V(row,col);
	static vectorN d(col);
	static matrixN U(row,col),Vt(col,row);

	int r;

	m = in;

	m.SVdecompose( d, V );

	Vt.transpose(V);
	
	U = m;
	
	for(r = 0; r < row ; r++)
	{
		if( d[r] < 0 )
		{
			for(int c = 0; c < 9; c++)
				Vt[r][c] = -Vt[r][c];
		}
	}
	out.mult(U,Vt);
}



//linear deformation 에 필요할듯.
void volumeConservation( matrix& A )
{
	float detA = A.det(), dets3;
	if( detA>0 )		dets3= 1.0f/powf( detA, 1/3.0f );
	else if( detA<0 )	dets3=-1.0f/powf(-detA, 1/3.0f );
	else				dets3= 100000000.0f;
	A = dets3*A;
}


//linear deformation 에 필요할듯.
/*
void volumeConservationN( matrixN& A )
{
	float detA = A.det(), dets3;
	if( detA>0 )		dets3= 1.0f/powf( detA, 1/3.0f );
	else if( detA<0 )	dets3=-1.0f/powf(-detA, 1/3.0f );
	else				dets3= 100000000.0f;
	A *= dets3;
}
*/
struct sample_MATERIAL{
 GLfloat ambient[3];
 GLfloat diffuse[3];
 GLfloat specular[3];
 GLfloat emission[3];
 GLfloat alpha;
 GLfloat phExp;
 int   texture;
};

extern int noFaces;
extern int noVertices;
extern int noMaterials;

extern short face_indicies[][3];
extern sample_MATERIAL materials [];
extern GLfloat vertices [][3];
extern int material_ref[][2];

// static buffer

const float dt = 0.033f;
const float alpha = 0.03f;
const float beta = 0.1f;
const float mu = 0.8f;
const float damp = 0.95f;
const float gravity = -10.0f;
const float init_height = 10.0f;
bool bQuad = false;
//#define QUAD

class MyApp: public Fl_3D_View
{
public:
	vector3* q;
	vectorN* q2;
	class position cm0;
	matrix Aqq;
	matrixN Aqq2;

	class position* x;
	class position* g;
	vector3* v;

	void	render( void );
	void	step( void );
	static int renderCB( void* ud ){ ((MyApp*)ud)->render(); return 1; }
	static void idelCB( void* ud );
	int handle( int );
	MyApp( void );
};


void MyApp::render( void )
{
	static vector3* normals = NULL;
	if( normals==NULL ) normals = new vector3[noVertices];
	int i, f;

	for( i=0; i<noVertices; i++ ) normals[i]=vector3(0,0,0);
	for( i=0; i<noMaterials; i++ )
	{
		for( f=material_ref[i][0]; f<material_ref[i][1]; f++ )
		{
			class position p0( x[face_indicies[f][0]] );
			class position p1( x[face_indicies[f][1]] );
			class position p2( x[face_indicies[f][2]] );
			vector3 n = normalize( (p1-p0)*(p2-p0) );
			normals[face_indicies[f][0]]+=n;
			normals[face_indicies[f][1]]+=n;
			normals[face_indicies[f][2]]+=n;
		}
	}
	for( i=0; i<noVertices; i++ ) normals[i].normalize();

	glUseProgramObjectARB( Fl_3D_View::plasticShader );
	for( i=0; i<noMaterials; i++ )
	{
		glSetMaterial( materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2], 1,
					   materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2], 1,
					   materials[i].specular[0], materials[i].specular[1], materials[i].specular[2], 1, materials[i].phExp );
		glBegin( GL_TRIANGLES );
		for( f=material_ref[i][0]; f<material_ref[i][1]; f++ )
		{
			class position p0( x[face_indicies[f][0]] );
			class position p1( x[face_indicies[f][1]] );
			class position p2( x[face_indicies[f][2]] );
			glNormal3f( normals[face_indicies[f][0]][0], normals[face_indicies[f][0]][1], normals[face_indicies[f][0]][2] );
			glVertex3f( p0[0], p0[1], p0[2] );
			glNormal3f( normals[face_indicies[f][1]][0], normals[face_indicies[f][1]][1], normals[face_indicies[f][1]][2] );
			glVertex3f( p1[0], p1[1], p1[2] );
			glNormal3f( normals[face_indicies[f][2]][0], normals[face_indicies[f][2]][1], normals[face_indicies[f][2]][2] );
			glVertex3f( p2[0], p2[1], p2[2] );
		}
		glEnd();
	}
	glUseProgramObjectARB( 0 );
}


vector3 * p = new vector3[noVertices];


void MyApp::step( void )
{
	// Update v and x here!!!
	int i;
	class position cm = class position(0,0,0);
	for( i=0; i<noVertices; i++ )	
		cm+=class vector3( x[i][0], x[i][1], x[i][2] )/(float)noVertices;

	//아래에서 왜 x[i] 가 바뀌지?
	for( i=0; i<noVertices; i++ )
        p[i] = class position( x[i][0], x[i][1], x[i][2] )-cm;


	if( bQuad == false)
	{
		matrix A,R;
		matrix Apq(0,0,0,0,0,0,0,0,0);
		for( i=0; i<noVertices; i++ )
		{
			Apq += matrix (p[i][0]*q[i][0], p[i][0]*q[i][1], p[i][0]*q[i][2], 
				p[i][1]*q[i][0], p[i][1]*q[i][1], p[i][1]*q[i][2], 
				p[i][2]*q[i][0], p[i][2]*q[i][1], p[i][2]*q[i][2] );
		}
		A = Apq*Aqq;
		orthogonalize(A,R);
		volumeConservation(A);
		matrix betaR = beta*A + (1.0f-beta)*R;

		for( i=0; i<noVertices; i++ )
			//g[i] = R*q[i] + cm;	//RIGID
			g[i] = betaR*q[i] + cm;	//Add Linear Transform

		// 골포지션이 0 보다 작은 애가 나오면 올려줘야 할까?+_+;
	}
	else
	{
		matrixN A2(3,9),R2(3,9);
		matrixN Apq2(3,9);
		for(int r = 0; r < 3 ; r++)
		{
			for( int c = 0; c < 9 ; c++)
			{
				Apq2[r][c] = 0.0f;
				A2[r][c] = 0.0f;
			}
		}
		for( i=0; i<noVertices; i++ )
		{
			for(int r = 0; r < 3 ; r++)
				for( int c = 0; c < 9 ; c++)
					Apq2[r][c] += p[i][r] * q2[i][c];
		}
		for(int r = 0; r < 9 ; r++)
			for( int c = 0; c < 9 ; c++)
				A2[r/3][c] += Apq2[r/3][c] * Aqq2[r][c];
/*
		matrix A,R;
		for(int r = 0; r < 3 ; r++)
		{
			for( int c = 0; c < 3 ; c++)
			{
				A[r][c] = A2[r][c];
			}
		}	

		orthogonalize(A,R);
		//volumeConservation(A);
*/
		orthogonalizeN(A2,R2);
		//volumeConservation(A2);
		//A2 *= beta;
		//R2 *= (1.0f-beta);
		//matrixN betaR(3,9);
		//betaR = A2;
		//A2 += R2;

		for( i=0; i<noVertices; i++ )
		{
			//g[i] = R2*q[i] + cm;	//RIGID
			for(int r = 0; r < 3 ; r++)
				for( int c = 0; c < 9 ; c++)
					g[i][r] += R2[r][c] * q[i][c] + cm[r];
		}
	}
	for( i=0; i<noVertices; i++ )
	{
		if ( i == 65 )
			int xfdsfd = 0;
		v[i] += alpha*( (g[i]-x[i])/dt );
		x[i] += dt*v[i];

		if( x[i][1] < 0.0f )
		{
			v[i][1] = -mu*(v[i][1]);
			x[i][1] = 0.0f;
		}
		else
			v[i][1] += dt*gravity;

		v[i] = damp*v[i];
	}

	redraw();
}


void MyApp::idelCB( void* ud )
{
	static DWORD last = GetTickCount();
	static int count = 0;

	if( GetTickCount()-last>dt*1000 )
	{
		last+=(DWORD)(dt*1000);	((MyApp*)ud)->step();
		if( ++count == 30 ){ cout<<"."; count=0; }
	}
	Fl::wait(0);
}


int MyApp::handle( int event )
{
	int i;
	switch( event )
	{
	case FL_KEYDOWN: 
		switch( Fl::event_key() )
		{	 
		case ' ':
		case FL_Enter:
		case FL_ENTER:
		case FL_Tab:
			for( i=0; i<noVertices; i++ )
			{
				//x[i][1] = 0;
				v[i] = vector3(0,gravity,0);
			}
		}
		return 1;
	case FL_KEYUP:	if( Fl::event_key()==' ' )
					{

					}
				   return 1;
	default	: return Fl_3D_View::handle( event );
	}
}

MyApp::MyApp( void ) : Fl_3D_View( 50, 50, 640, 480, "Meshless Deformation" )
{
	int i;
	//크기 10배로 확대.??
	for( i=0; i<noVertices; i++ )	vertices[i][0]*=10, vertices[i][1]*=10, vertices[i][2]*=10;

	q = new vector3[noVertices];
	q2 = new vectorN[noVertices];
	x = new class position[noVertices];
	v = new vector3[noVertices];
	g = new class position[noVertices];

	//무게 중심 구하기.
	cm0=class position(0,0,0);
	for( i=0; i<noVertices; i++ )	
		cm0+=vector3(vertices[i][0], vertices[i][1], vertices[i][2] )/(float)noVertices;

	for( i=0; i<noVertices; i++ )
	{
		q[i] = class position( vertices[i][0], vertices[i][1], vertices[i][2] )-cm0;
		q2[i].setSize(9);
		q2[i][0] = q[i][0];
		q2[i][1] = q[i][1];
		q2[i][2] = q[i][2];
		q2[i][3] = q[i][0]*q[i][0];
		q2[i][4] = q[i][1]*q[i][1];
		q2[i][5] = q[i][2]*q[i][2];
		q2[i][6] = q[i][0]*q[i][1];
		q2[i][7] = q[i][1]*q[i][2];
		q2[i][8] = q[i][2]*q[i][0];
		x[i] = class position( vertices[i][0], vertices[i][1], vertices[i][2] );
		x[i] = rotate( exp( X_axis*M_PI/6/2 ), x[i] )+ vector3( 0, init_height, 0 );
		v[i] = vector3(0,0,0);
	}
	matrix IAqq(0,0,0,0,0,0,0,0,0);
	matrixN IAqq2(9,9);
	for(int r = 0; r < 9 ; r++)
		for( int c = 0; c < 9 ; c++)
			IAqq2[r][c] = 0;
	for( i=0; i<noVertices; i++ )
	{
		IAqq += matrix (q[i][0]*q[i][0], q[i][0]*q[i][1], q[i][0]*q[i][2], 
						q[i][1]*q[i][0], q[i][1]*q[i][1], q[i][1]*q[i][2], 
						q[i][2]*q[i][0], q[i][2]*q[i][1], q[i][2]*q[i][2] );

		for(int r = 0; r < 9 ; r++)
			for( int c = 0; c < 9 ; c++)
				IAqq2[r][c] += q2[r][c]*q2[c][r];
	}
	Aqq = inverse( IAqq );	
	IAqq2.LUinverse(Aqq2);
	resizable( *this );
	setDrawCallback( renderCB, this );
	show();
	Fl::add_idle( idelCB, this );

}

int _tmain(int argc, _TCHAR* argv[])
{
	MyApp* app = new MyApp();
	if ( argc >= 2 )
	{
		if ( strstr(argv[1],"/quad") )
			bQuad = true;

	}
	Fl::run();
	return 0;
}


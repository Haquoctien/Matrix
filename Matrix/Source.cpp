#include "matrix.h"

int main(void)
{
	matrix matA(3, 3);
	matA.matIn();
	//matB.matIn();
	matrix matB = matA * 2;
	matB.matOut();
	(matB.inverse()).matOut();
	/*(matB.minor(1, 2)).matOut();
	cout << "Dinh thuc cua ma tran vuong la: " << matB.det() << endl;*/
	system("pause");
	return 0;
}
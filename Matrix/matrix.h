#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

using namespace std;

#define flip(x, y)      \
    {                   \
        float temp = x; \
        x = y;          \
        y = temp;       \
    }
#define numOut(x) cout << setw(7) << setfill(' ') << fixed << setprecision(3) << x;
class matrix
{
private:
	int m;
	int n;
	float **mat;
public:
	matrix(int, int);
	~matrix();
	int &getRowNumber();
	int &getColNumber();
	float **&getMat();
	void matIn();
	void matOut();
	matrix(const matrix &other);
	matrix operator=(matrix &other);
	matrix operator+(matrix &other);
	float *&operator[](unsigned int i);
	matrix operator-(matrix &other);
	matrix operator*(matrix &other);
	matrix operator*(const double &i);
	matrix transpose();
	matrix flipRow(unsigned int, unsigned int);
	matrix minor(unsigned int, unsigned int);
	float det();
	matrix echelonMatOf();
	matrix inverse();
};

matrix::matrix(int a, int b)
{
	m = a;
	n = b;
	mat = new float *[m];
	for (int i = 0; i < m; i++)
		mat[i] = new float[n];
}

void matrix::matIn()
{
	for (int i = 0; i < m; i++)
	{
		cout << "Nhap tung phan tu cua dong:" << endl;
		for (int j = 0; j < n; j++)
			cin >> (*this)[i][j];
	}
}

void matrix::matOut()
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			numOut((*this)[i][j]);
		cout << endl;
	}
	cout << endl;
}

int &matrix::getRowNumber()
{
	return m;
}

int &matrix::getColNumber()
{
	return n;
}

float **&matrix::getMat()
{
	return this->mat;
}

matrix::~matrix()
{
	for (int i = 0; i < m; i++)
		delete[] mat[i];
	delete[] mat;
}

matrix::matrix(const matrix &other)
{
	m = other.m;
	n = other.n;
	mat = new float *[m];
	for (int i = 0; i < m; i++)
	{
		mat[i] = new float[n];
		for (int j = 0; j < n; j++)
			mat[i][j] = other.mat[i][j];
	}
}

matrix matrix::operator=(matrix &other)
{
	matrix &self = *this;
	if (mat != NULL)
	{
		for (int i = 0; i < m; i++)
			delete[] self[i];
		delete[] self.getMat();
	}
	mat = new float *[m];
	for (int i = 0; i < m; i++)
	{
		self[i] = new float[n];
		for (int j = 0; j < n; j++)
			self[i][j] = other[i][j];
	}
	return self;
}

float *&matrix::operator[](unsigned int i)
{
	return this->mat[i];
}

matrix matrix::operator-(matrix &other)
{
	matrix &self = *this;
	matrix ans = matrix(other.m, other.n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans[i][j] = self[i][j] - other[i][j];
	return ans;
}

matrix matrix::operator+(matrix &other)
{
	matrix &self = *this;
	matrix ans = matrix(other.m, other.n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans[i][j] = self[i][j] + other[i][j];
	return ans;
}

matrix matrix::operator*(const double &a)
{
	matrix &self = *this;
	matrix ans = matrix(self.getRowNumber(), self.getColNumber());
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans[i][j] = a * (self[i][j]);
	return ans;
}

matrix matrix::operator*(matrix &other)
{
	matrix matAns = matrix(other.m, other.n), &self = *this;
	float temp;
	for (int i = 0; i < matAns.getRowNumber(); i++)
		for (int j = 0; j < matAns.getColNumber(); j++)
		{
			temp = 0;
			for (int r = 0; r < self.getColNumber(); r++)
				temp += self[i][r] * other[r][j];
			matAns[i][j] = temp;
		}
	return matAns;
}

matrix matrix::transpose()
{
	matrix &self = *this;
	matrix ans(self.getColNumber(), self.getRowNumber());
	for (int i = 0; i < self.getColNumber(); i++)
		for (int j = 0; j < self.getRowNumber(); j++)
			ans[i][j] = self[j][i];
	return ans;
}

matrix matrix::flipRow(unsigned int i, unsigned int ip)
{
	matrix &self = *this;
	matrix ans(self.getRowNumber(), self.getColNumber());
	for (int j = 0; j < self.getColNumber(); j++)
		flip(ans[i][j], ans[ip][j]);
	return ans;
}

matrix matrix::minor(unsigned int ip, unsigned int jp)
{
	matrix &self = *this;
	matrix ans(self.getRowNumber() - 1, self.getColNumber() - 1);
	int ans_i = 0, ans_j = 0;
	for (int i = 0; i < self.getRowNumber(); i++)
	{
		if (i == ip)
			continue;
		ans_j = 0;
		for (int j = 0; j < self.getColNumber(); j++)
		{
			if (j == jp)
				continue;
			ans[ans_i][ans_j] = self[i][j];
			ans_j++;
		}
		ans_i++;
	}
	return ans;
}

float matrix::det()
{
	matrix &self = *this;
	if (self.getRowNumber() == 1)
		return self[0][0];
	if (self.getRowNumber() == 2)
		return self[0][0] * self[1][1] - self[0][1] * self[1][0];
	int sign = 1;
	float ans = 0;
	for (int i = 0; i < self.getRowNumber(); i++)
	{
		if (self[i][0] != 0)
			ans += sign * self[i][0] * (self.minor(i, 0)).det();
		sign = -sign;
	}
	return ans;
}

matrix matrix::echelonMatOf()
{
	// ma trận Amn có m dòng n cột
	matrix matAns = *this;
	// đi qua mỗi cột thứ j, chạy j: 0 -> n-1 (phần tử trụ sẽ là ajj)
	for (int j = 0; j < n; j++)
	{
		// cho là các phần tử của cột j mà i > j đều bằng 0
		bool all0Col = true;
		// kiểm tra cột có toàn 0
		for (int i = j; i < m; i++)
		{
			float ele = matAns[i][j];
			// nếu có phần tử khác 0
			if (ele != 0)
				all0Col = false;
			// nếu có phần tử khác 0 và nếu phần tử ajj = 0, đổi hàng j với hàng i
			if (!all0Col && matAns[j][j] == 0)
			{
				matAns.flipRow(i, j);
				// kết thúc vòng lặp
				break;
			}
		}
		// nếu tất cả các phần tử của cột j mà i > j đều bằng 0, bỏ qua cột này
		if (all0Col)
			continue;
		// nếu không
		// đi qua mỗi dòng thứ i, chạy i: j+1 -> m-1
		for (int i = j + 1; i < m; i++)
		{
			// lấy hệ số x : hi = hi -x*hj
			float x = (matAns[i][j] / matAns[j][j]);
			// đi qua mỗi phần tử của dòng, chạy jp: 0 -> n-1
			for (int jp = 0; jp < n; jp++)
				matAns[i][jp] -= x * matAns[j][jp];
		}
	}
	return matAns;
}

matrix matrix::inverse()
{
	matrix& self = *this;
	matrix matAns(self.getRowNumber(), self.getColNumber());
	for (int i = 0; i < self.getRowNumber(); i++)
		for (int j = 0; j < self.getColNumber(); j++)
		{
			matAns[i][j] = (float)((i + j) % 2 ? -1 : 1)*((self.minor(i, j)).det());
		}
	matAns.transpose();
	return matAns*(1.0/self.det());
}

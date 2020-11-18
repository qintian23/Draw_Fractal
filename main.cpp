// �������ƣ�����ѧ - ��������Ŵ�� Mandelbrot Set (���²����ؼ�)
// ���뻷����Visual C++ 6.0��EasyX 2011���ݰ�
// �����£�2010-9-9
//
#include <graphics.h>
#include <conio.h>

// ���峣��
#define ITERATIONS 1000		// ����������Խ�ߣ�ͼ��Խ��ϸ
#define MAXCOLOR 64			// ��ɫ��


/////////////////////////////////////////////////
// ���帴�����ˡ�������
/////////////////////////////////////////////////

// ���帴��
struct COMPLEX
{
	double re;
	double im;
};

// ���帴�����ˡ�����
COMPLEX operator * (COMPLEX a, COMPLEX b)
{
	COMPLEX c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.im * b.re + a.re * b.im;
	return c;
}

// ���帴�����ӡ�����
COMPLEX operator + (COMPLEX a, COMPLEX b)
{
	COMPLEX c;
	c.re = a.re + b.re;
	c.im = a.im + b.im;
	return c;
}


/////////////////////////////////////////////////
// ������ɫ����ʼ����ɫ
/////////////////////////////////////////////////

// ������ɫ
int Color[MAXCOLOR];

// ��ʼ����ɫ
void InitColor()
{
	// ʹ�� HSL ��ɫģʽ�����Ƕ� h1 �� h2 �Ľ���ɫ
	int h1 = 240, h2 = 30;
	for (int i = 0; i < MAXCOLOR / 2; i++)
	{
		Color[i] = HSLtoRGB((float)h1, 1.0f, i * 2.0f / MAXCOLOR);
		Color[MAXCOLOR - 1 - i] = HSLtoRGB((float)h2, 1.0f, i * 2.0f / MAXCOLOR);
	}
}


/////////////////////////////////////////////////
// ���� Mandelbrot Set (���²����ؼ�)
/////////////////////////////////////////////////
void Draw(double fromx, double fromy, double tox, double toy)
{
	COMPLEX z, c;
	int x, y, k;	// ����ѭ������
	for (x = 0; x < 640; x++)
	{
		c.re = fromx + (tox - fromx) * (x / 640.0);
		for (y = 0; y < 480; y++)
		{
			c.im = fromy + (toy - fromy) * (y / 480.0);
			z.re = z.im = 0;
			for (k = 0; k < ITERATIONS; k++)
			{
				if (z.re * z.re + z.im * z.im > 4.0)	break;
				z = z * z + c;
			}
			putpixel(x, y, (k >= ITERATIONS) ? 0 : Color[k % MAXCOLOR]);
		}
	}
}


/////////////////////////////////////////////////
// ������
/////////////////////////////////////////////////
void main()
{
	// ��ʼ����ͼ���ڼ���ɫ
	initgraph(640, 480);
	InitColor();


	// ��ʼ�� Mandelbrot Set(���²����ؼ�)����ϵ
	double fromx, fromy, tox, toy;
	fromx = -2.1; tox = 1.1;
	fromy = -1.2; toy = 1.2;
	Draw(fromx, fromy, tox, toy);


	// ������������ʵ�ַŴ����ѡ������
	MOUSEMSG m;
	bool isLDown = false;
	int selfx, selfy, seltx, selty;	// ����ѡ��

	while (!_kbhit())
	{
		m = GetMouseMsg();			// ��ȡһ�������Ϣ

		switch (m.uMsg)
		{
			// ������м��ָ�ԭͼ������ϵ
		case WM_MBUTTONUP:
			fromx = -2.1; tox = 1.1;
			fromy = -1.2; toy = 1.2;
			Draw(fromx, fromy, tox, toy);
			break;

			// �����������϶���ѡ������
		case WM_MOUSEMOVE:
			if (isLDown)
			{
				rectangle(selfx, selfy, seltx, selty);
				seltx = m.x;
				selty = m.y;
				rectangle(selfx, selfy, seltx, selty);
			}
			break;

			// �����������϶���ѡ������
		case WM_LBUTTONDOWN:
			setcolor(WHITE);
			setwritemode(R2_XORPEN);
			isLDown = true;
			selfx = seltx = m.x;
			selfy = selty = m.y;
			rectangle(selfx, selfy, seltx, selty);

			break;

			// �����������϶���ѡ������
		case WM_LBUTTONUP:
			rectangle(selfx, selfy, seltx, selty);
			setwritemode(R2_COPYPEN);
			isLDown = false;
			seltx = m.x;
			selty = m.y;

			if (selfx == seltx || selfy == selty)	break;

			// ����ѡ��Ϊ 4:3
			int tmp;
			if (selfx > seltx) { tmp = selfx; selfx = seltx; seltx = tmp; }
			if (selfy > selty) { tmp = selfy; selfy = selty; selty = tmp; }

			if ((seltx - selfx) * 0.75 < (selty - selfy))
			{
				selty += (3 - (selty - selfy) % 3);
				selfx -= (selty - selfy) / 3 * 4 / 2 - (seltx - selfx) / 2;
				seltx = selfx + (selty - selfy) / 3 * 4;
			}
			else
			{
				seltx += (4 - (seltx - selfx) % 4);
				selfy -= (seltx - selfx) * 3 / 4 / 2 - (selty - selfy) / 2;
				selty = selfy + (seltx - selfx) * 3 / 4;
			}

			// ��������ϵ
			double f, t;
			f = fromx + (tox - fromx) * selfx / 640;
			t = fromx + (tox - fromx) * seltx / 640;
			fromx = f;
			tox = t;
			f = fromy + (toy - fromy) * selfy / 480;
			t = fromy + (toy - fromy) * selty / 480;
			fromy = f;
			toy = t;

			// ��ͼ��
			Draw(fromx, fromy, tox, toy);

			break;
		}
	}

	_getch();
	closegraph();
}


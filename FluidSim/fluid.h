#pragma once
#include<algorithm>
#include<math.h>

/* Simulating vector fields for non compressible fluids */
/* Inspired by "Stable Fluids" - Jos Stam, 1999 */

const int sizex = 300;
const int sizey = 300;

const int PS = 2;

class field
{
public:
    
    double vx[sizey][sizex];
    double vy[sizey][sizex];

};

class scalar
{
public:
    double val[sizey][sizex];
};

void initField(field *f)
{
    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            f->vx[y][x] = 20 * ((double)y-12) * sin(5);
            f->vy[y][x] = 20 * ((double)x-12) * cos(5);
        }
    }
}

void addForce(field *f0, field *f, field *force, double k)
{
    // v(t+k) = v(t) + k * f(t)
    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            f0->vx[y][x] = f->vx[y][x] + force->vx[y][x] * k;
            f0->vy[y][x] = f->vy[y][x] + force->vy[y][x] * k;
        }
    }
}

void traceback(field* trace, field* f, double k)
{
    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            trace->vx[y][x] = std::min((double)sizex-2, std::max((double)0, (double)x - k * f->vx[y][x]));
            trace->vy[y][x] = std::min((double)sizey-2, std::max((double)0, (double)y - k * f->vy[y][x]));
        }
    }
}

void advectField(field *trace, field *f)
{
    int xi, yi;
    double xf, yf, i1, i2;

    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            xi = (int)trace->vx[y][x];
            yi = (int)trace->vy[y][x];

            xf = trace->vx[y][x] - xi;
            yf = trace->vy[y][x] - yi;

            i1 = f->vx[yi][xi] * (1 - xf) + f->vx[yi][xi + 1] * xf;
            i2 = f->vx[yi + 1][xi] * (1 - xf) + f->vx[yi + 1][xi + 1] * xf;

            f->vx[y][x] = i1 * (1 - yf) + i2 * yf;

            i1 = f->vy[yi][xi] * (1 - xf) + f->vy[yi][xi + 1] * xf;
            i2 = f->vy[yi + 1][xi] * (1 - xf) + f->vy[yi + 1][xi + 1] * xf;

            f->vy[y][x] = i1 * (1 - yf) + i2 * yf;
        }
    }
}

void advectScalar(field *trace, scalar* s)
{
    scalar* temp = (scalar*)calloc(1, sizeof(scalar));

    int xi, yi;
    double xf, yf, i1, i2;

    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            xi = (int)trace->vx[y][x];
            yi = (int)trace->vy[y][x];

            xf = trace->vx[y][x] - xi;
            yf = trace->vy[y][x] - yi;

            i1 = (s->val[yi][xi] * (1 - xf)) + (s->val[yi][xi + 1] * xf);
            i2 = (s->val[yi + 1][xi] * (1 - xf)) + (s->val[yi + 1][xi + 1] * xf);

            temp->val[y][x] = (i1 * (1 - yf)) + (i2 * yf);
        }
    }

    *s = *temp;
    free(temp);
    temp = NULL;

}

void diffuseField(field *f, double k, int steps)
{
    field* fp = (field*)calloc(1, sizeof(field));

    // Gauss-Siedel method for a linear system
    for (int s = 0; s < steps; s++)
    {
        for (int y = 1; y < sizey - 1; y++)
        {
            for (int x = 1; x < sizex - 1; x++)
            {
                fp->vx[y][x] = (k * ( fp->vx[y - 1][x] + fp->vx[y + 1][x] + fp->vx[y][x - 1] + fp->vx[y][x + 1] ) + f->vx[y][x]) / (1 + 4*k);
                fp->vy[y][x] = (k * ( fp->vy[y - 1][x] + fp->vy[y + 1][x] + fp->vy[y][x - 1] + fp->vy[y][x + 1] ) + f->vy[y][x]) / (1 + 4*k);
            }
        }
    }

    *f = *fp;
    free(fp);
    fp = NULL;
}

void diffuseScalar(scalar* sl, double k, int steps)
{
    scalar* sp = (scalar*)calloc(1, sizeof(scalar));

    // Gauss-Siedel method for a linear system

    for (int s = 0; s < steps; s++)
    {
        for (int y = 1; y < sizey - 1; y++)
        {
            for (int x = 1; x < sizex - 1; x++)
            {
                sp->val[y][x] = ( k * ( sp->val[y - 1][x] + sp->val[y + 1][x] + sp->val[y][x - 1] + sp->val[y][x + 1] ) + sl->val[y][x]) / (1 + 4*k);
            }
        }
    }
    *sl = *sp;
    free(sp);
    sp = NULL;
}

void projectField(field *f, int steps = 20)
{
    // calculating div(v)
    double* div = (double*)calloc(sizey * sizex, sizeof(double));
     
    double* p = (double*)calloc(sizey * sizex, sizeof(double));


    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            if (div)
                div[y*sizex + x] = - (0.5/(sizex-2)) * (f->vx[y][x + 1] - f->vx[y][x - 1] + f->vy[y + 1][x] - f->vy[y - 1][x]);
        }
    }
    for (int step = 0; step < steps; step++)
    {
        for (int y = 1; y < sizey - 1; y++)
        {
            for (int x = 1; x < sizex - 1; x++)
            {
                p[y * sizex + x] = (div[y * sizex + x] + p[y * sizex + x - 1] + p[y * sizex + x + 1] + p[(y - 1) * sizex + x] + p[(y + 1) * sizex + x]) / 4;
            }
        }
    }
    free(div);
    div = NULL;

    for (int y = 1; y < sizey - 1; y++)
    {
        for (int x = 1; x < sizex - 1; x++)
        {
            f->vx[y][x] -= 0.5 * (sizex-2) * (p[y*sizex + x + 1] - p[y * sizex + x - 1]);
            f->vy[y][x] -= 0.5 * (sizex-2) * (p[(y + 1) * sizex + x] - p[(y - 1) * sizex + x]);
        }
    }

    free(p);
    p = NULL;
}

void swapFields(field *f0, field *f1)
{
    double tx, ty;
    for (int y = 0; y < sizey; y++)
    {
        for (int x = 0; x < sizex; x++)
        {
            tx = f0->vx[y][x];
            ty = f0->vy[y][x];

            f0->vx[y][x] = f1->vx[y][x];
            f0->vy[y][x] = f1->vx[y][x];

            f1->vx[y][x] = tx;
            f1->vy[y][x] = ty;
        }
    }
}

void bnd(field* f)
{
    for (int x = 1; x < sizex - 1; x++)
    {
        //f->vy[0][x] = abs(f->vy[1][x]);
        f->vy[1][x] = 0;

        //f->vy[sizey - 1][x] = -abs(f->vy[sizey-2][x]);
        f->vy[sizey - 2][x] = 0;

    }

    for (int y = 1; y < sizey - 1; y++)
    {
        //f->vx[y][0] = abs(f->vx[y][1]);
        f->vx[y][1] = 0;

        //f->vx[y][sizex - 1] = -abs(f->vx[y][sizex - 2]);
        f->vx[y][sizex - 2] = 0;

        

    }
}

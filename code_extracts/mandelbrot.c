/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbianco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 01:49:55 by gbianco           #+#    #+#             */
/*   Updated: 2019/04/26 01:57:51 by gbianco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "colors.h"

void	put_pixel_mdl(t_container *c, int *pixel, int *rep, double *mu)
{
	*mu = *mu / c->chan.rep;
	*rep = *mu * 255;
	*(int*)(c->view.str + *pixel) = mandel_calc_col(c, rep);
	*(char*)(c->view.str + *pixel + 3) = 0;
}

void	cplx_to_pxl_mandel(t_container *c, t_complex *mbr, int *x, int *y)
{
	int			rep;
	int			pixel;
	double		mu;
	t_point		sqr;

	sqr.x = 0;
	sqr.y = 0;
	rep = -1;
	while (++rep < c->chan.rep && sqr.x + sqr.y <= 4)
	{
		mbr->z.y = mbr->z.x * mbr->z.y;
		mbr->z.y += mbr->z.y;
		mbr->z.y += mbr->c.y;
		mbr->z.x = sqr.x - sqr.y + mbr->c.x;
		sqr.x = mbr->z.x * mbr->z.x;
		sqr.y = mbr->z.y * mbr->z.y;
	}
	pixel = (*x + *y * WIDTH) << 2;
	if (rep == c->chan.rep)
		*((int *)(c->view.str + pixel)) = c->chan.frctclr;
	else
	{
		mu = rep + 1 - log(log(sqr.x + sqr.y)) / 0.693147;
		put_pixel_mdl(c, &pixel, &rep, &mu);
	}
}

void	*mandelbrot_points(void *ct)
{
	t_container	*c;
	t_complex	mbr;
	int			x;
	int			y;

	c = (t_container*)ct;
	y = c->calc.start - 1;
	while (++y < c->calc.end)
	{
		mbr.c.y = (y * (0.0052) - 2) * c->chan.zoom + c->chan.y;
		x = -1;
		while (++x < WIDTH)
		{
			mbr.c.x = (x * (0.0052) - 3.3) * c->chan.zoom + c->chan.x;
			mbr.z.x = 0;
			mbr.z.y = 0;
			cplx_to_pxl_mandel(c, &mbr, &x, &y);
		}
	}
	pthread_exit(NULL);
}

void	mandelbrot_threads(t_container *c)
{
	pthread_t	thread[THREAD];
	int			i;

	i = 0;
	while (i < THREAD)
	{
		c[i].chan = c->chan;
		pthread_create(&thread[i], NULL, mandelbrot_points, &c[i]);
		i++;
	}
	while (i > 0)
	{
		pthread_join(thread[i - 1], NULL);
		i--;
	}
	mlx_put_image_to_window(c->view.mlx, c->view.win, c->view.img, 0, 0);
}

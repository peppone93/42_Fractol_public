/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbianco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 02:56:24 by gbianco           #+#    #+#             */
/*   Updated: 2019/04/25 23:38:39 by gbianco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "colors.h"

void	put_pixel(t_container *c, int *x, int *y, int *rep)
{
	int pixel;

	if (*x < WIDTH && *y < HEIGH && *x >= 0 && *y >= 0)
	{
		pixel = (*x + *y * WIDTH) << 2;
		if (*rep == c->chan.rep)
			*((int *)(c->view.str + pixel)) = c->chan.frctclr;
		else
		{
			*(int*)(c->view.str + pixel) = julia_calc_col(c, rep);
			*(char*)(c->view.str + pixel + 3) = 0;
		}
	}
}

void	complex_to_pixel(t_container *c, t_complex *jul, int *x, int *y)
{
	int			rep;
	double		ty;
	t_point		sqr;

	ty = jul->z.y;
	sqr.x = jul->z.x * jul->z.x;
	sqr.y = ty * ty;
	rep = -1;
	while (++rep < c->chan.rep && sqr.x + sqr.y <= 4)
	{
		ty = jul->z.x * ty;
		ty += ty;
		ty += jul->c.y;
		jul->z.x = sqr.x - sqr.y + jul->c.x;
		sqr.x = jul->z.x * jul->z.x;
		sqr.y = ty * ty;
	}
	put_pixel(c, x, y, &rep);
}

void	*julia_points(void *ct)
{
	t_container	*c;
	t_complex	jul;
	int			x;
	int			y;

	c = (t_container*)ct;
	jul.c.x = -0.7269 + c->chan.real;
	jul.c.y = 0.1889 + c->chan.imma;
	y = c->calc.start - 1;
	while (++y < c->calc.end)
	{
		x = -1;
		jul.z.y = (y * (0.0052) - 2) * c->chan.zoom + c->chan.y;
		while (++x < WIDTH)
		{
			jul.z.x = (x * (0.0052) - 2.657) * c->chan.zoom + c->chan.x;
			complex_to_pixel(c, &jul, &x, &y);
		}
	}
	pthread_exit(NULL);
}

void	julia_threads(t_container *c)
{
	pthread_t	thread[THREAD];
	int			i;

	i = 0;
	while (i < THREAD)
	{
		c[i].chan = c->chan;
		pthread_create(&thread[i], NULL, julia_points, &c[i]);
		i++;
	}
	while (i > 0)
	{
		pthread_join(thread[i - 1], NULL);
		i--;
	}
	mlx_put_image_to_window(c->view.mlx, c->view.win, c->view.img, 0, 0);
}

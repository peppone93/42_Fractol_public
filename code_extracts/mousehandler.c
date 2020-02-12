/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mousehandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbianco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:27:04 by gbianco           #+#    #+#             */
/*   Updated: 2019/04/29 16:23:48 by gbianco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "handlers.h"

int		close_hook(void)
{
	exit(EXIT_SUCCESS);
	return (0);
}

void	position_zoom(int *x, int *y, t_container *c)
{
	double	tx;
	double	ty;
	double	pertx;
	double	perty;

	tx = *x - 512;
	pertx = tx / 192;
	ty = *y - 384;
	perty = ty / 192;
	if (cfr(c, "dragoncurve") == 1 || cfr(c, "sierpinskicarpet") == 1)
	{
		c->chan.x -= (tx * 0.004) * c->chan.zoom;
		c->chan.y -= (ty * 0.004) * c->chan.zoom;
	}
	else
	{
		c->chan.x += (pertx * 0.19) * c->chan.zoom;
		c->chan.y += (perty * 0.19) * c->chan.zoom;
	}
}

int		mouse_press_hook(int button, int x, int y, t_container *c)
{
	if (button == SCRUP)
		c->chan.zoom *= 0.9;
	if (button == SCRDW)
		c->chan.zoom *= 1.1;
	if (button == SCRUP || button == SCRDW)
	{
		position_zoom(&x, &y, c);
		check_right_fractal(c);
	}
	return (0);
}

int		mouse_moves_hook(int x, int y, t_container *c)
{
	if (c->chan.special == 0)
	{
		c->chan.real = (x - 512) * 0.002;
		c->chan.imma = (y - 384) * 0.002;
		julia_threads(c);
	}
	return (0);
}

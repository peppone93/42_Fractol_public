/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbianco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 16:40:06 by gbianco           #+#    #+#             */
/*   Updated: 2019/04/27 01:56:50 by gbianco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "colors.h"

void	additional_ite_information(t_container *c, char *cnbr)
{
	if (cfr(c, "julia") == 1 || cfr(c, "sierpinskicarpet") == 1)
	{
		if (cfr(c, "julia") == 1)
			mlx_string_put(c->view.mlx, c->view.inwin, 5, 54, WHITE,
					"Frozen image:");
		else
			mlx_string_put(c->view.mlx, c->view.inwin, 5, 54, WHITE,
					"Filled squares:");
		if (c->chan.special == 0)
			mlx_string_put(c->view.mlx, c->view.inwin, 204, 54, ORANGE, "OFF");
		else
			mlx_string_put(c->view.mlx, c->view.inwin, 204, 54, GREEN, "ON");
	}
	else if (ft_strcmp(c->view.fractal, "multibrot") == 0)
	{
		cnbr = ft_itoa(c->view.n);
		mlx_string_put(c->view.mlx, c->view.inwin, 5, 54, WHITE,
				"Exponent:       ");
		mlx_string_put(c->view.mlx, c->view.inwin, 205, 54, WHITE, cnbr);
		ft_strdel(&cnbr);
	}
}

void	iteractive_information(t_container *c)
{
	char	*cnbr;

	cnbr = ft_itoa(c->chan.i + 1);
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 36, WHITE,
			"Color scheme:        /6");
	mlx_string_put(c->view.mlx, c->view.inwin, 205, 36, WHITE, cnbr);
	ft_strdel(&cnbr);
	iteractive_iterations(c);
	additional_ite_information(c, cnbr);
	iteractive_zoom(c);
}

void	general_commands(t_container *c)
{
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 88, RED,
			"Controls");
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 106, WHITE,
			"Zoom:");
	mlx_string_put(c->view.mlx, c->view.inwin, 204, 106, BBLUE,
			"[-],[+] or (wheel)");
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 124, WHITE,
			"Movements:");
	mlx_string_put(c->view.mlx, c->view.inwin, 204, 124, BBLUE,
			"[arrows]");
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 142, WHITE,
			"Inc/Dec iterations:");
	mlx_string_put(c->view.mlx, c->view.inwin, 204, 142, BBLUE,
			"[1],[2]");
	mlx_string_put(c->view.mlx, c->view.inwin, 5, 160, WHITE,
			"Change colors:");
	mlx_string_put(c->view.mlx, c->view.inwin, 204, 160, BBLUE,
			"[c]");
}

void	commands(t_container *c)
{
	general_commands(c);
	if (cfr(c, "julia") == 1)
	{
		mlx_string_put(c->view.mlx, c->view.inwin, 5, 178, WHITE,
				"Fre/Unfreeze image:");
		mlx_string_put(c->view.mlx, c->view.inwin, 204, 178, BBLUE,
				"[f]");
	}
	if (cfr(c, "sierpinskicarpet") == 1)
	{
		mlx_string_put(c->view.mlx, c->view.inwin, 5, 178, WHITE,
				"Fill/Empty squares:");
		mlx_string_put(c->view.mlx, c->view.inwin, 204, 178, BBLUE,
				"[f]");
	}
}

void	put_instructions(t_container *c)
{
	void	*img;

	if (!(img = mlx_new_image(c->view.mlx, 420, 210)))
		error(c);
	mlx_put_image_to_window(c->view.mlx, c->view.inwin, img, 0, 0);
	commands(c);
	iteractive_information(c);
	mlx_destroy_image(c->view.mlx, img);
}

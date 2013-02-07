
#include "engine.h"
#include "piece.h"
#include "pieces_define.h"
#include "block.h"
#include "board.h"

piece_s new_piece(piece_e type)
{
//	if ((type < 0) || (type >= PIECE_MAX))
//		exit(0); /* Isn't that a bit rough? */
	piece_s p;
	color_e color;

	switch(type)
	{
	case PIECE_S: color = BLACK_GREEN;   break;
	case PIECE_Z: color = BLACK_RED;     break;
	case PIECE_O: color = BLACK_YELLOW | A_BOLD; break;
	case PIECE_I: color = BLACK_CYAN   | A_BOLD; break;
	case PIECE_L: color = BLACK_YELLOW;  break;
	case PIECE_J: color = BLACK_BLUE;    break;
	case PIECE_T: color = BLACK_MAGENTA; break;
	default:      color = WHITE_BLACK;
	}

	p.rotation = 0;
	p.type     = type;
	p.color    = color;
	p.theme[0] = ' ';
	p.theme[1] = ' ';
	p.theme[2] = '\0';

	/* If we're creating a dummy piece, it won't be printed anyway */
	if (!piece_is_valid(&p))
		return p;

	p.x = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	p.y = global_pieces_position[p.type][p.rotation][1];

	/* This seems complicated, but it's just starting each
	 * block of the piece according to it's x and y on the board */
	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p.type][p.rotation][j][i] != 0)
			{
				int block_x  = p.x + i;
				int block_y  = p.y + j;

				p.block[k] = new_block(block_x, block_y, p.theme, p.color);
				k++;
			}
	return p;
}

/** Rotate piece #p by #rotation times. Negative number rotates backwards */
void piece_rotate(piece_s* p, int rotation)
{
	if (rotation < 0)
		rotation += 4;
	p->rotation = (p->rotation + rotation) % 4; /* keep it under 4 */

	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				p->block[k] = new_block(p->x + i, p->y + j, p->theme, p->color);
				k++;
			}
}

/** Moves piece according to #direction. 0 is right and 1 is left */
void piece_move(piece_s* p, direction_e dir)
{
	int delta_x = 0;
	int delta_y = 0;

	switch (dir)
	{
	case DIR_LEFT:  delta_x = -1; break;
	case DIR_RIGHT: delta_x =  1; break;
	case DIR_DOWN:  delta_y =  1; break;
	case DIR_UP:    delta_y = -1; break;
	default: return;
	}

	p->x += delta_x;
	p->y += delta_y;

	int k;
	for (k = 0; k < 4; k++)
	{
		p->block[k].x += delta_x;
		p->block[k].y += delta_y;
	}
}

/** Drops piece as far as possible */
void piece_hard_drop(piece_s* p, board_s* b)
{
	while (piece_can_move(p, b, DIR_DOWN))
		piece_move(p, DIR_DOWN);
}

/** Returns a random piece */
piece_e piece_get_random()
{
	int min = 0;
	int max = PIECE_MAX - 1;
	return rand() % (max - min + 1) + min;
}

/** Checks if the piece #p can move to direction #dir.
 *  The thing here is we create a 'dummy' piece, move it and then
 *  tests if it's on a right spot.
 */
bool piece_can_move(piece_s* p, board_s* b, direction_e dir)
{
	piece_s new_p = *p;

	piece_move(&new_p, dir);

	/* Going through the board only on the positions of the piece's blocks */
	int k;
	for (k = 0; k < 4; k++)
	{
		/* Here we don't confuse our 'dummy' blocks with real ones */
		new_p.block[k].type = EMPTY;

		/* block's x and y are not relative to the piece -- they're global */
		int block_x = new_p.block[k].x;
		int block_y = new_p.block[k].y;

		/* Off-limits check */
		if ((block_x >= BOARD_WIDTH) || (block_y >= BOARD_HEIGHT) ||
			(block_x < 0))
			return false;

		/* If the piece is still out of the board, we don't check collision */
		if (block_y < 0) continue;

		/* Fellow blocks check */
		if (b->block[block_x][block_y].type != EMPTY)
			return false;
	}
	return true;
}

bool piece_can_rotate(piece_s* p, board_s* b, int rotation)
{
	piece_s new_p = *p;

	piece_rotate(&new_p, rotation);
	return piece_can_move(&new_p, b, DIR_NONE);
}

/** Reset piece's state - right on top and with no rotation */
void piece_reset(piece_s* p)
{
	if (!piece_is_valid(p))
		return;

	p->rotation = 0;
	p->x = BOARD_WIDTH/2 + global_pieces_position[p->type][p->rotation][0];
	p->y = global_pieces_position[p->type][p->rotation][1];

	/* This seems complicated, but it's just starting each
	 * block of the piece according to it's x and y on the board */
	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				int block_x  = p->x + i;
				int block_y  = p->y + j;

				p->block[k] = new_block(block_x, block_y, p->theme, p->color);
				k++;
			}
}

/** Tells if a piece is a basic one (one of the 7 original ones) */
bool piece_is_valid(piece_s* p)
{
	if ((p->type < 0) || (p->type >= PIECE_MAX))
		return false;
	else
		return true;
}

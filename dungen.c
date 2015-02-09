#include "dungen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int		x_inbound(t_dungeon *this, const int x) {
	return x >= 0 && x < this->_size_x;
}

int		y_inbound(t_dungeon *this, const int y) {
	return y >= 0 && y < this->_size_y;
}

void	set_tile(t_dungeon *this, const int x, const int y, const t_tile tile) {
	if (this->x_inbound(this, x) && this->y_inbound(this, y)) {
		this->_data[x * this->_size_x * y] = tile;
	}
}

void 	set_tiles(t_dungeon *this, int startx, int starty, int endx, int endy, t_tile tile) {
	if ((this->x_inbound(this, startx) && this->x_inbound(this, endx))
		&& (this->y_inbound(this, starty) && this->y_inbound(this, endy))
		&& (startx <= endx && starty <= endy)) {
		for (int y = starty; y != endy + 1; ++y) {
			for (int x = startx; x != endx + 1; ++x) {
				this->set_tile(this, x, y, tile);
			}
		}
	}
}


t_tile	get_tile(t_dungeon *this, const int x, const int y) {
	if (this->x_inbound(this, x) && this->y_inbound(this, y)) {
		return this->_data[x * this->_size_x * y];
	}
	return -1;
}

int		area_used(t_dungeon *this, const int startx, const int starty, const int endx, const int endy) {
	if ((this->x_inbound(this, startx) && this->x_inbound(this, endx))
		&& (this->y_inbound(this, starty) && this->y_inbound(this, endy))
		&& (startx <= endx && starty <= endy)) {
		for (int y = starty; y != endy + 1; y++) {
			for (int x = startx; x != endx + 1; x++) {
				if (this->get_tile(this, x, y) != UNUSED)
					return 0;
			}
		}
	}
	return 1;
}

int		adjacent(t_dungeon *this, const int x, const int y, const t_tile tile) {
	if ((this->x_inbound(this, x - 1) && this->x_inbound(this, x + 1))
		&& (this->y_inbound(this, y - 1) && this->y_inbound(this, y + 1))) {
		return this->get_tile(this, x - 1, y) == tile || this->get_tile(this, x + 1, y) == tile
			|| this->get_tile(this, x, y - 1) == tile || this->get_tile(this, x, y + 1) == tile;
	}
	return -1;
}

void	dump(t_dungeon *this) {
	for (int y = 0; y != this->_size_y; y++) {
		for (int x = 0; x != this->_size_x; x++) {
			switch(this->get_tile(this, x, y)) {
			case UNUSED:
				printf(" ");
				break;
			case WALL:
				printf("#");
				break;
			case FLOOR:
				printf(".");
				break;
			case CORRIDOR:
				printf(".");
				break;
			case DOOR:
				printf("+");
				break;
			case IN:
				printf("<");
				break;
			case OUT:
				printf(">");
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}

int		_dungeon_init_wrapper(t_di_wrapper args) {
	int x = args.x ? args.x : 25;
	int y = args.y = args.y ? args.y : 25;
	t_tile default_tile = args.tile ? args.tile : UNUSED;

	return _dungeon_init(args.this, x, y, default_tile); 
}

int		_dungeon_init(t_dungeon *this, const int x, const int y, const t_tile tile) {
	this->_size_x = x;
	this->_size_y = y;
	this->set_tile = &set_tile;
	this->set_tiles = &set_tiles;
	this->get_tile = &get_tile;
	this->x_inbound = &x_inbound;
	this->y_inbound = &y_inbound;
	this->area_used = &area_used;
	this->adjacent = &adjacent;
	this->dump = &dump;
	if ((this->_data = malloc((this->_size_x * this->_size_y) * sizeof(int))) == NULL) {
		fprintf(stderr, ERROR_DATA_ALLOCATION, strerror(errno));
		exit(1);
	}
	memset(&this->_data, tile, sizeof(this->_data));
	return 0;
}

void	t_dungeon_destroy(t_dungeon *this) {
	free(this->_data);
}

t_dungeon 	build_dungeon(t_dungen *this, t_dungeon *dung, const nt rng) {
	
}

t_dungeon	generate(t_dungen *this) {
	if ((this->max_features > 0 && this->max_features <= 100)
		&& (this->size_x > 3 && this->size_x <= 80)
		&& (this->size_y > 3 && this->size_y <= 30)) {
		int rng = rand();
		t_dungeon dung;
		t_dungeon_init(this->size_x, this->size_y);
		this->build_dungeon(this, &dung, rng);
	}
}

int		_dungen_init_wrapper(t_d_wrapper args) {
	int	seed = args.seed ? args.seed : 0;
	int x = args.x ? args.x : 80;
	int y = args.y ? args.y : 30;
	int features = args.features ? args.features : 100;
	int room = args.room ? args.room : 75;
	int corridor = args.corridor ? args.corridor : 25;
	return _dungen_init(args.this, seed, x, y, features, room, corridor);
}

int		_dungen_init(t_dungen *this, const int seed, const int x, const int y,
					 const int features, const int room, const int corridor) {
	this->seed = seed;
	this->size_x = x;
	this->size_y = y;
	this->max_features = features;
	this->room_chance = room;
	this->corridor_chance = corridor;
	this->generate = &generate;
	this->build_dungeon = &build_dungeon;
	srand(this->seed);
	return 0;
}

int		main(void) {
	t_dungeon dung;

	t_dungeon_init(&dung, 20, 20);
	printf("%d %d", dung._size_x, dung._size_y);
}

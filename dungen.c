#include "dungen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

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
		return this->_data[x + this->_size_x * y];
	}
	return -1;
}

int		area_used(t_dungeon *this, const int startx, const int starty, const int endx, const int endy) {
	if ((this->x_inbound(this, startx) && this->x_inbound(this, endx))
		&& (this->y_inbound(this, starty) && this->y_inbound(this, endy))
		&& (startx <= endx && starty <= endy)) {
		for (int y = starty; y != endy + 1; y++) {
			for (int x = startx; x != endx + 1; x++) {
				printf("pointer address is %p\n", this->_data);
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

t_dungeon		*t_dungeon_init(const int x, const int y, const t_tile tile) {
	t_dungeon	*tmp = NULL;

	if ((tmp = malloc(sizeof(t_dungeon) + sizeof(int) * x * y)) == NULL) {
		fprintf(stderr, ERROR_DATA_ALLOCATION, strerror(errno));
		exit(1);
	}
	memset(&tmp->_data, tile, x * y * sizeof(int));
	tmp->_size_x = x;
	tmp->_size_y = y;
	tmp->_data = (int*)(tmp+1);
	tmp->set_tile = &set_tile;
	tmp->set_tiles = &set_tiles;
	tmp->get_tile = &get_tile;
	tmp->x_inbound = &x_inbound;
	tmp->y_inbound = &y_inbound;
	tmp->area_used = &area_used;
	tmp->adjacent = &adjacent;
	tmp->dump = &dump;
	return tmp;
}

void	t_dungeon_destroy(t_dungeon *this) {
	free(this->_data);
}

static int		get_random_int(const int min, const int max) {
	int rng = rand() % max;
	return rng >= min ? rng : min;
}

int		build_room(t_dungen *this, t_dungeon *dungeon, const int x, const int y,
				   const t_direction dir) {
	int	len_x = get_random_int(4, MAX_ROOM_LEN_X);
	int	len_y = get_random_int(4, MAX_ROOM_LEN_Y);

	int start_x = x;
	int start_y = y;
	int end_x = x;
	int end_y = y;

	if (dir == NORTH) {
		start_y = y - len_y;
		start_x = x - len_x / 2;
		end_x = x + (len_x + 1) / 2;
	} else if (dir == EAST) {
		start_y = y - len_y / 2;
		end_y = y + (len_y + 1) / 2;
		end_x = x + len_x;
	} else if (dir == SOUTH) {
		end_y = y + len_y;
		start_x = x - len_x / 2;
		end_x = x + (len_x + 1) / 2;
	} else if (dir == WEST) {
		start_y = y - len_y / 2;
		end_y = y + (len_y + 1) / 2;
		start_x = x - len_x;
	}

	if (!dungeon->x_inbound(dungeon, start_x) || !dungeon->x_inbound(dungeon, end_x)
		|| !dungeon->y_inbound(dungeon, start_y) || !dungeon->y_inbound(dungeon, end_y)) {
		return 0;
	}
	if (!dungeon->area_used(dungeon, start_x, start_y, end_x, end_y)) {
		return 0;
	}
	dungeon->set_tiles(dungeon, start_x, start_y, end_x, end_y, WALL);
	dungeon->set_tiles(dungeon, start_x + 1, start_y + 1, end_x - 1, end_y - 1, FLOOR);
	return 1;
}

static t_direction get_dir() {
	return rand() % 4;
}

int	   build_corridor(t_dungen *this, t_dungeon *dungeon, int x, int y, int max_len, t_direction direction) {
	if ((x >= 0 && x < this->size_x)
		&& (y >= 0 && y < this->size_y)
		&& (max_len > 0 && max_len <= MAX(this->size_x, this->size_y))) {

		int len = get_random_int(2, max_len);
		int start_x = x;
		int start_y = y;
		int end_x = x;
		int end_y = y;
	
		if (direction == NORTH)
			start_y = y - len;
		else if (direction == EAST)
			end_x = x + len;
		else if (direction == SOUTH)
			end_y = y + len;
		else if (direction == WEST)
			start_x = x - len;
		
		if (!dungeon->x_inbound(dungeon, start_x)  || !dungeon->x_inbound(dungeon, end_x)
			|| !dungeon->y_inbound(dungeon, start_y) || !dungeon->y_inbound(dungeon, end_y))
			return 1;
		if (!dungeon->area_used(dungeon, start_x, start_y, end_x, end_y))
			return 0;
		dungeon->set_tiles(dungeon, start_x, start_y, end_x, end_y, CORRIDOR);
		return 1;
	}
	return 0;
}

int 	build_feature(t_dungen *this, t_dungeon *dungeon, int x, int y, int xmod, int ymod, t_direction direction) {
	int chance = get_random_int(0, 100);
	
	if (chance <= this->room_chance) {
		if (this->build_room(this, dungeon, x + xmod, y + ymod, direction)) {
			dungeon->set_tile(dungeon, x, y, DOOR);
			dungeon->set_tile(dungeon, x + xmod, y + ymod, FLOOR);			
			return 1;
		}
		return 0;
	}
	else {
		if (this->build_corridor(this, dungeon, x + xmod, y + ymod, 6, direction)) {
			dungeon->set_tile(dungeon, x, y, DOOR);
			return 1;
		}
		return 0;
	}
	return 0;
}

int			build_features(t_dungen *this, t_dungeon *dungeon) {
	for (int try = 0; try != MAX_FEATURE_TRY; try++) {
		int x = get_random_int(1, this->size_x - 2);
		int y = get_random_int(1, this->size_y - 2);
		
		if (dungeon->get_tile(dungeon, x, y) != WALL && dungeon->get_tile(dungeon, x, y) != CORRIDOR) {
			continue;
		}
		if (dungeon->adjacent(dungeon, x, y, DOOR)) {
			continue;
		}

		if (dungeon->get_tile(dungeon, x, y + 1) == FLOOR || dungeon->get_tile(dungeon, x, y + 1) == CORRIDOR) {
			if (this->build_feature(this, dungeon, x, y, 0, -1, NORTH))
				return 1;
		}
		else if (dungeon->get_tile(dungeon, x-1, y) == FLOOR || dungeon->get_tile(dungeon, x-1, y) == CORRIDOR) {
			if (build_feature(this, dungeon, x, y, 1, 0, EAST))
				return 1;
		}
		else if (dungeon->get_tile(dungeon, x, y-1) == FLOOR || dungeon->get_tile(dungeon, x, y-1) == CORRIDOR) {
			if (build_feature(this, dungeon, x, y, 0, 1, SOUTH))
				return 1;
		}
		else if (dungeon->get_tile(dungeon, x+1, y) == FLOOR || dungeon->get_tile(dungeon, x+1, y) == CORRIDOR) {
			if (build_feature(this, dungeon, x, y, -1, 0, WEST))
				return 1;
		}
	}
	return 0;
}

int			build_stairs(t_dungen *this, t_dungeon *dungeon, const t_tile tile) {
	for (int try = 0 ; try != MAX_STAIR_TRY; try++) {
		int x = get_random_int(1, this->size_x - 2);
		int y = get_random_int(1, this->size_x - 2);
		
		if (!dungeon->adjacent(dungeon, x, y, FLOOR) && !dungeon->adjacent(dungeon, x, y, CORRIDOR))
			continue;
		if (dungeon->adjacent(dungeon, x, y, DOOR))
			continue;
		
		dungeon->set_tile(dungeon, x, y, tile);
		return 1;
	}
 	return 0;
}

void 	build_dungeon(t_dungen *this, t_dungeon *dung) {
	this->build_room(this, dung, this->size_x / 2, this->size_y / 2, get_dir());
	for (int features = 1; features != this->max_features; features++) {
		if (!this->build_features(this, dung)) {
			fprintf(stderr, WARN_NO_MORE_ROOM_FOR_FEATURE);
			break;
		}
	}
	
	if (!this->build_stairs(this, dung, OUT))
		fprintf(stderr, "oups");
	if (!this->build_stairs(this, dung, IN))
		fprintf(stderr, "oups");
}

t_dungeon	*generate(t_dungen *this) {
	t_dungeon *dung = NULL;
	if ((this->max_features > 0 && this->max_features <= 100)
		&& (this->size_x > 3 && this->size_x <= 80)
		&& (this->size_y > 3 && this->size_y <= 30)) {
		dung = t_dungeon_init(this->size_x, this->size_y, UNUSED);
		printf("freshly build pointer address is %p\n", dung->_data);
		this->build_dungeon(this, dung);
	}
	return dung;
}

int		t_dungen_init(t_dungen *this, const int x, const int y,
					 const int features, const int room, const int corridor) {
	this->size_x = x;
	this->size_y = y;
	this->max_features = features;
	this->room_chance = room;
	this->corridor_chance = corridor;
	this->generate = &generate;
	this->build_dungeon = &build_dungeon;
	this->build_room = &build_room;
	this->build_features = &build_features;
	this->build_feature = &build_feature;
	this->build_corridor = &build_corridor;
	this->build_stairs = &build_stairs;
	srand(time(NULL));
	return 0;
}

int		main(void) {
	t_dungen dungen;

	t_dungen_init(&dungen, 80, 25, 100, 75, 25);
	t_dungeon *dungeon = dungen.generate(&dungen);
	dungeon->dump(dungeon);
}

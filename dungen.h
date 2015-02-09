#ifndef		DUNGEN_H_
# define	DUNGEN_H_

# define 		ERROR_DATA_ALLOCATION "Error with memory allocation %s\n"

typedef enum	e_tile {
	UNUSED,
	WALL,
	FLOOR,
	CORRIDOR,
	DOOR,
	IN,
	OUT
} 				t_tile;

typedef enum	e_direction {
	NOTH,
	SOUTH,
	EAST,
	WEST
}				t_direction;	

typedef struct	s_dungeon {
	int			_size_x;
	int 		_size_y;
	int			*_data;

	void		(*set_tile)(struct s_dungeon *this, const int x, const int y, const t_tile tile);
	void		(*set_tiles)(struct s_dungeon *this, const int startx, const int starty, const int endx,
							 const int endy, t_tile tile);
	t_tile		(*get_tile)(struct s_dungeon *this, const int x, const int y);
	int			(*x_inbound)(struct s_dungeon *this, const int x);
	int			(*y_inbound)(struct s_dungeon *this, const int y);
	int			(*area_used)(struct s_dungeon *this, const int startx, const int starty, const int endx, const int endy);
	int			(*adjacent)(struct s_dungeon *this, const int x, const int y, const t_tile tile);
	void   		(*dump)(struct s_dungeon *this);
}				t_dungeon;

typedef struct	s_dungen {
	int			seed;
	int			size_x;
	int 		size_y;
	int 		max_features;
	int 		room_chance;
	int 		corridor_chance;

	t_dungeon	(*generate)(struct s_dungen *this);
	t_dungeon	(*build_dungeon)(struct s_dungen *this, t_dungeon *dungeon, const int rng);
}				t_dungen;

typedef struct 	s_di_wrapper {
	t_dungeon	*this;
	int			x;
	int			y;
	t_tile		tile;
}				t_di_wrapper;

typedef struct 	s_d_wrapper {
	t_dungen	*this;
	int			seed;
	int			x;
	int			y;
	int 		features;
	int			room;
	int			corridor;
}				t_d_wrapper;

# define		t_dungeon_init(...) _dungeon_init_wrapper((t_di_wrapper){__VA_ARGS__});
int				_dungeon_init_wrapper(t_di_wrapper args);
int			   	_dungeon_init(t_dungeon *this, const int x, const int y, const t_tile tile);
void			t_dungeon_destroy(t_dungeon *this);
# define		t_dungen_init(...) _dungen_init_wrapper((t_d_wrapper){__VA_ARGS__});
int				_dungen_init_wrapper(t_d_wrapper args);
int				_dungen_init(t_dungen *this, const int seed, const int x, const int y,
							 const int features, const int room, const int corridor);
#endif		/* !DUNGEN_H_ */

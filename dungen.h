#ifndef		DUNGEN_H_
# define	DUNGEN_H_

# define 		ERROR_DATA_ALLOCATION			"Error with memory allocation %s\n"
# define		WARN_NO_MORE_ROOM_FOR_FEATURE 	"Unable to place more features in dungeon\n"
# define		MAX_ROOM_LEN_X		  			8
# define		MAX_ROOM_LEN_Y					6
# define		MAX_FEATURE_TRY					999
# define		MAX_STAIR_TRY					9999

# define 		MAX(a, b) (a > b ? a : b)

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
	NORTH,
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
	int			size_x;
	int 		size_y;
	int 		max_features;
	int 		room_chance;
	int 		corridor_chance;

	t_dungeon	*(*generate)(struct s_dungen *this);
	void		(*build_dungeon)(struct s_dungen *this, t_dungeon *dungeon);
	int			(*build_room)(struct s_dungen *this, t_dungeon *dungeon, const int x, const int y,
							  const t_direction dir);
	int			(*build_features)(struct s_dungen *this, t_dungeon *dungeon);
	int			(*build_feature)(struct s_dungen *this, t_dungeon *dungeon, const int x, const int y,
								 const int xmod, const int ymod, const t_direction direction);
	int			(*build_corridor)(struct s_dungen *this, t_dungeon *dungeon, const int x, const int y,
								  const int max_len, t_direction direction);
	int			(*build_stairs)(struct s_dungen *this, t_dungeon *dungeon, const t_tile tile);
}				t_dungen;

t_dungeon	    *t_dungeon_init(const int x, const int y, const t_tile tile);
void			t_dungeon_destroy(t_dungeon *this);
int				t_dungen_init(t_dungen *this, const int x, const int y,
							 const int features, const int room, const int corridor);
#endif		/* !DUNGEN_H_ */

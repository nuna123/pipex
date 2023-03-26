NAME			=	pipex

OBJ_DIR			=	objs/
CFLAGS			=	-Wall -Werror -Wextra 
#Sources
SRC_FILES		=	pipex\
					piper\
					get_full_cmd\
					pipex_utils\
					prog_creation


LIBFT_FOL		= Libft/
LIBFT			= ${addprefix ${LIBFT_FOL}, libft.a}

SRCS			=	${addprefix ${SRC_DIR}, ${addsuffix .c, ${SRC_FILES}}}
BONUS_SRCS			=	${addprefix ${SRC_DIR}, ${addsuffix .c, ${BONUS_SRC_FILES}}}

# Colors
NRM_COLOR		=	\033[0;39m

GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m

#################################################
#####	RULES	#################################
#################################################

all:		${NAME}

${NAME}:	${SRCS} ${LIBFT}
			@ cc ${CFLAGS} ${SRCS} ${LIBFT} -I${LIBFT_FOL} -o ${NAME}
			@ echo "${GREEN}pipex compiled!${NRM_COLOR}"

bonus:		${BONUS_SRCS}
			@ cc ${CFLAGS} ${BONUS_SRCS} ${LIBFT} -I${LIBFT_FOL} -o ${NAME}
			@ echo "${GREEN}BONUS pipex compiled!${NRM_COLOR}"

${LIBFT}:	
			@ make -C ${LIBFT_FOL} re all bonus extra --no-print-directory --silent
			@ echo "${GREEN}Libft compiled :))${NRM_COLOR}"

clean:
			@ make -C ${LIBFT_FOL} clean --no-print-directory --silent
			@ echo "${YELLOW}cleaning Libft..."
			@ rm -rf ${OBJ_DIR}
			@ echo "deleting ${OBJ_DIR}..."

fclean:
			@ make -C ${LIBFT_FOL} fclean --no-print-directory --silent
			@ echo "${YELLOW}cleaning Libft..."
			@ rm -rf ${OBJ_DIR}
			@ echo "deleting ${OBJ_DIR}..."
			@ rm -f ${NAME}
			@ echo "deleting ${NAME}..."

re:		fclean all

.PHONY:		all clean fclean re bonus

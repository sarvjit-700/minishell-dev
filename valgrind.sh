#valgrind --trace-children=yes \
#--log-fd=9 9>>/home/ksorokol/GitHub/minishell/vg.log \
#--leak-check=full \
#--show-leak-kinds=all \
#--suppressions=/home/ksorokol/GitHub/minishell/rl.supp \
#./minishell
# --trace-children=yes 

valgrind  --leak-check=full --log-file=valgrind.log  --track-origins=yes --show-leak-kinds=all --suppressions=valgrind.supp ./minishell

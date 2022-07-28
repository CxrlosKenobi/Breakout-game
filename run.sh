###
# Run.sh -- Our cool shortcut to run SDL2 packages at once.
###
runner="gcc main.c -o Breakout -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm";
 
clear;
echo -e "\e[35m[ \e[33m-\e[90m ] \e[33mCompiling ... \e[35m";

echo -e "\e[90m[ $ ] $runner \n\e[35m";
eval $runner;

if [ $? -eq 0 ]; then
  echo -e "[ \e[32m+\e[90m ] \e[0mCompiled succesfully! \e[0m";
  echo -e "\e[35m[ \e[33m-\e[90m ] \e[33mRunning ... \e[35m";
  ./Breakout;
else
  echo -e "\n[ \e[31m!\e[90m ] \e[31mCompilation failed. \e[35m";

fi;

echo -e "\n[ \e[32m+\e[90m ] \e[0mClosed. \e[0m";
exit 0;

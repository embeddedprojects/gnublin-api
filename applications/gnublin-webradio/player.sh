#! /bin/sh

## Start and Stop mpg123 for Webradio

case "$1" in

    start)
        echo "Player start"
        mpg123 --fifo mpg_pipe -R > mpg.log 2> mpg_err.log &
        echo $! > mpg.pid
        ;;
    stop)
        echo "Player stop"
        if [ -e mpg.pid ]
        then
            if ( kill -TERM $(cat mpg.pid) 2> /dev/null )
            then
                c=1
                while [ "$c" -le 30 ]; do
                  if ( kill -0 $(cat mpg.pid) 2> /dev/null )
                  then
                      sleep 1
                  else
                      break
                  fi
                  c=$((++c))
                done
             fi
             if ( kill -0 $(cat mpg.pid) 2> /dev/null )
             then
                 kill -KILL $(cat mpg.pid)
             fi
        fi
        rm mpg.pid
        ;;
esac

exit 0

#! /usr/bin/env ruby
loop do  
  system('clear')
  system('ps -M | grep -v ruby | grep -v fish')
  sleep 0.5
end

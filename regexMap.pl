#!/usr/bin/perl
use strict;
use warnings;
use boolean;
#Author: Francesco
#Description: This script was used for testing the validity of the map files stored inside the maps directory of the game. Like I have explained in the report, this script was simply used for testing. The code was then converted in C++ and included in the source code of the Game within the MapfilesHandler class.

my @array;
foreach my$name(@ARGV){
undef @array;
open (my $RR,"<", "$name") or die "Error: $!";

while(<$RR>){
    push(@array,$_);
}
#chomp @array;
print "filename is $name\n";
print "@array";
my $counter=1;
my $linenumber=1;
my $startpattern=false;
my $stoppattern=false;
my $notvalid=false;
my $indexstart=0;
my $indexstop=0;

for(my $i=1; $i<@array; $i++){
    $linenumber++;
    if($startpattern){
        if($array[$i] =~ /^\*{17}$/){
           $indexstop=$i; 
           $stoppattern=true;
           last;
        }    
        if($array[$i]=~/^\*[0-3 \*\+\-]{15}\*$/){
                $counter++; ## check if map is in fomat *-----* or ********* and set not valid
                next;
            }else{
                $notvalid=true;
                last;
            }
    }else{
        if($i >= int($#array/2)){
            $notvalid=true;
            last;
        }else{
            if(!($array[$i] =~ /[\*\-]{17}/)){
                $notvalid=true;
                last;
            }else{
                if($array[$i] =~ /^\*{17}$/){
                    $indexstart=$i;
                    $startpattern=true;
                }
            }
        }
    }
}

    close $RR;
if($startpattern && $stoppattern){
    my $offset=$indexstop - $indexstart;
    if($offset >= 8 && $offset <=12){
        print "Map is Valid\n";
    }else{
        print "Map is not valid error found in line: $linenumber\n";
    }
}
}

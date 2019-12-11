#! /usr/bin/env perl
#
# common.pm: Common subroutine library
#
# Unchanged in Masper - MonoTrain
#
# Version 1.1  13.10.99  Borge Lindberg
#                        Conversion of 2 changed to ox to avoid clash with
#                        Danish diphthong
#			 grep avoided
#
# Version 1.0  13.07.98  Finn Tore Johansen
#
###############################################################################

sub sampa2htk {
#
# Replace problematic symbols in the SAMPA phoneme code by "safe" symbols.
# The safe symbols have been checked against the official SAMPA
# symbols and should be OK for Danish, Dutch, English, German, Italian,
# Norwegian, Swedish, Spanish and Hungarian if no prosodic markers are used.
#
# The SAMPA characters considered for these languages are: a-z A-Z : { } & @ ?
# 
# Possible remaining problems are:
#   French:     e~ a~ o~ 9~ E/ A/ &/ O/ U~/
#   Portuguese: i~ e~ 6~ o~ u~ 6~j~
#   Bulgarian:  p' b' t' d' k' g' ts' dz' f' v' s' z' x' m' n' l' r'
#   Estonian:   7 77 t' t't s' s's n' n'n l' l'l 
#   Polish:     e~ o~ s' z' ts' dz' n'
#   Romanian:   i_0 e_X o_X
#   Croatian:   tS' dZ' v\ S' Z'
#   Russian:    p' b' t' d' k' g' tS' t-s t-S' f' v' s' z' S': x' m' n' l' r'
#
    local($tmp) = @_;
    $tmp =~ s/\{/ae/g;
    $tmp =~ s/2/ox/g;
    $tmp =~ s/3/Eh/g;
    $tmp =~ s/6/ah/g;
    $tmp =~ s/9/oe/g;
    $tmp =~ s/\&/OE/g;
    $tmp =~ s/\@/eh/g;
    $tmp =~ s/\}/uh/g;
    $tmp =~ s/\?/qq/g;
    return $tmp;
}

sub run { # run unix shell command with error check
    local($cmd) = @_;
    die "Unix command failed:\n $cmd\n $!\n" if system "$cmd";
}

sub printfile { # create new file and print string to it
    local($fn,$string) = @_;
    open(FILE,"> $fn") || die "Could not create $fn: $!\n";
    print FILE $string;
    close(FILE);
}

sub appendfile { # append string to existing file
    local($fn,$string) = @_;
    open(FILE,">> $fn") || die "Could not open $fn: $!\n";
    print FILE $string;
    close(FILE);
}

sub getdir { # return file names in directory, not including hidden files
    my($dir) = @_;
    opendir(DIR,$dir) || die "Could not open directory $dir";
    my(@fns) = readdir(DIR);
    closedir(DIR);
    my(@newfns) = ();
    while ($pop = pop(@fns))
    { unless ($pop =~ /^\./) { @newfns = ($pop, @newfns);}}
    return (@newfns);	# remove filenames starting with a . 
}

sub makedir { # create directory if it doeasn't already exist
    local($dir) = @_;
    &run ("mkdir -p $dir") unless (-d $dir);
}

1;

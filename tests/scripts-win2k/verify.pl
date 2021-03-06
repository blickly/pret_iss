eval "exec perl -S $0 $*"
  if 0;
# *****************************************************************************
#
#  The following code is derived, directly or indirectly, from the SystemC
#  source code Copyright (c) 1996-2008 by all Contributors.
#  All Rights reserved.
#
#  The contents of this file are subject to the restrictions and limitations
#  set forth in the SystemC Open Source License Version 2.3 (the "License");
#  You may not use this file except in compliance with such restrictions and
#  limitations. You may obtain instructions on how to receive a copy of the
#  License at http://www.systemc.org/. Software distributed by Contributors
#  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
#  ANY KIND, either express or implied. See the License for the specific
#  language governing rights and limitations under the License.
#
# *****************************************************************************
#
#  verify.pl - Regression test script for SystemC 2.0 (and higher).
#
#  Original Author: Martin Janssen, Synopsys, Inc., 2001-01-18
#
# *****************************************************************************
#
#  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
#  changes you are making here.
#
#      Name, Affiliation, Date: David Long, Doulos Ltd., 2005-05-04
#  Description of Modification: Added support for MSCV60 and MSVC71 running 
#                               under Windows2000
#
# *****************************************************************************

# use calling directory for include path
$0 =~ m|^(.*)/|;
push( @INC, $1 );

# set the path to the test suite
&set_systemc_test( $1 );

# flush STDOUT after each print
$oldfh = select( STDOUT ); $| = 1; select( $oldfh );

&main;


# -----------------------------------------------------------------------------
#  SUB : set_systemc_test
#
#  Set the path to the test suite, either specified by environment variable
#  SYSTEMC_TEST, or derived from the location of this script.
# -----------------------------------------------------------------------------

sub set_systemc_test
{
    local( $dir ) = @_;

    if( defined $ENV{ 'SYSTEMC_TEST' } ) {
        $rt_systemc_test = $ENV{ 'SYSTEMC_TEST' };
    }
    else {
        local( $wdir );
        chop( $wdir = `pwd` );
        chdir( "$dir/.." );
        chop( $rt_systemc_test = `pwd` );
        chdir( "$wdir" );
    }
}


# -----------------------------------------------------------------------------
#  SUB : print_log
#
#  Print routine used instead of 'print' as it allows dupping to output file
#  ($rt_output_file).
# -----------------------------------------------------------------------------

sub print_log
{
    print STDOUT @_;

    if( defined $rt_output_file ) {
        if( ! defined $RT_OUTPUT_FH ) {
            local( $f ) = "$rt_output_dir/$rt_output_file";
            $RT_OUTPUT_FH = $f;
            open( RT_OUTPUT_FH, ">$f" ) ||
                die "Error: cannot open output file '$f'\n";
            # chmod( $rt_file_permissions, "$f" );
        }
        print RT_OUTPUT_FH @_;
    }
}


# -----------------------------------------------------------------------------
#  SUB : create_mail
#
#  Create mail with test results.
# -----------------------------------------------------------------------------

sub create_mail
{
    local( $mail_file ) = "$rt_output_dir/verify.mail";
    open( MAIL, ">$mail_file" ) ||
        die "Error: cannot open mail file '$mail_file'\n";
    # chmod( $rt_file_permissions, "$mail_file" );

    local( $host );
    chop( $host = `hostname` );
    local( $uname_a );
    chop( $uname_a = `uname -a` );
    local( $date );
    chop( $date = `date` );

    local( $pass ) = $#rt_pass + 1;
    local( $fail ) = $#rt_fail + 1;

    printf MAIL "\n";
    printf MAIL "REGRESSION RESULTS for SystemC on %s\n", $host;

    printf MAIL "\n";
    printf MAIL "%s\n", $uname_a;
    printf MAIL "%s\n", $date;

    printf MAIL "\n";
    printf MAIL "SYSTEMC_ARCH : %s\n", $rt_systemc_arch;
    printf MAIL "SYSTEMC_HOME : %s\n", $rt_systemc_home;
    printf MAIL "SYSTEMC_TEST : %s\n", $rt_systemc_test;
    printf MAIL " OUTPUT_DIR  : %s\n", $rt_output_dir;

    printf MAIL "\n";
    printf MAIL "Total passed: %d\n", $pass;
    printf MAIL "Total failed: %d\n", $fail;

    printf MAIL "\n";
    printf MAIL "---------------------------------------" .
                "----------------------------------------\n";
    printf MAIL " Tests that passed :\n";
    printf MAIL "---------------------------------------" .
                "----------------------------------------\n";
    foreach $tmp ( @rt_pass ) {
        printf MAIL "%s\n", $tmp;
    }

    printf MAIL "\n";
    printf MAIL "---------------------------------------" .
                "----------------------------------------\n";
    printf MAIL " Tests that failed :\n";
    printf MAIL "---------------------------------------" .
                "----------------------------------------\n";
    foreach $tmp ( @rt_fail ) {
        printf MAIL "%s\n", $tmp;
    }

    printf MAIL "\n";
    
    close( MAIL );

    $mail_file;
}


# -----------------------------------------------------------------------------
#  SUB : send_mail
#
#  Send mail with test results.
# -----------------------------------------------------------------------------

sub send_mail
{
    local( $mail_file ) = @_;

    local( $header );

$header = <<"EOM";
From: $rt_email_admin (Regression Administrator)
To: $rt_email_dist (Regress Distribution List)
Subject: Regress Report - SystemC
EOM

    local( $mail_prog ) = ( -x "/usr/lib/sendmail" ) ?
        "/usr/lib/sendmail" :
            "/bin/mail";

    open( RT_MAIL_H, "| $mail_prog $rt_email_dist") || 
        die "Error: cannot open mail program '$mail_prog'\n";

    print RT_MAIL_H $header;

    local( $body ) = `cat $mail_file`;
    print RT_MAIL_H $body;

    close( RT_MAIL_H );
}


# -----------------------------------------------------------------------------
#  SUB : compile_results
#
#  Print summary of passing and failing tests.
# -----------------------------------------------------------------------------

sub compile_results
{
    &print_log( "\n*** test results\n" );

    &print_log( "\n" );
    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );
    &print_log( " Tests that passed :\n" );
    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );
    foreach $tmp ( @rt_pass ) {
        &print_log( "$tmp\n" );
    }

    &print_log( "\n" );
    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );
    &print_log( " Tests that failed :\n" );
    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );
    foreach $tmp ( @rt_fail ) {
        &print_log( "$tmp\n" );
    }

    local( $pass ) = $#rt_pass + 1;
    local( $fail ) = $#rt_fail + 1;

    &print_log( "\n" );
    &print_log( "Total passed : " . ( $pass ) . "\n" );
    &print_log( "Total failed : " . ( $fail ) . "\n" );

    use integer;
    local( $gut ) = ( $pass * 100 ) / ( $pass + $fail );

    &print_log( "\n" );
    &print_log( "   GUT METER : " . ( $gut ) . " %\n" );

    local( $script );
    ( $script = $0 ) =~ s|^.*/||;
    local( $host );
    chop( $host = `hostname` );
    local( $uname_a );
    chop( $uname_a = `uname -a` );
    local( $date );
    chop( $date = `date` );

    &print_log( "\n*** $script finished successfully on host $host\n\n" );
    &print_log( "$uname_a\n" );
    &print_log( "$date\n" );
    &print_log( "\n" );

    if( $rt_mail ) {
        local( $mail_file );
        $mail_file = &create_mail;
        &send_mail( $mail_file );
    }
}


# -----------------------------------------------------------------------------
#  SUB : interrupt_handler
#
#  Routine for handling the SIGINT and SIGQUIT signals.
# -----------------------------------------------------------------------------

sub interrupt_handler
{
    local( $signal ) = @_;

    &print_log( "caught SIG$signal\n" );

    if( defined $rt_child_pid ) {
        &print_log( "killing process '$rt_child_pid'\n" );
        kill 'TERM', $rt_child_pid;
    }

    &compile_results;

    exit 1;
}


# -----------------------------------------------------------------------------
#  SUB : alarm_handler
#
#  Routine for handling the SIGALRM signal.
# -----------------------------------------------------------------------------

sub alarm_handler
{
    local( $signal ) = @_;

    &print_log( "caught SIG$signal\n" );

    # kill process and group of processes
    &print_log( "timing out process '$rt_child_pid'\n" );
    kill 'TERM', $rt_child_pid;

    # keep track of timed out tests
    push( @rt_alarm, $rt_current_test );

    # reset the SIGALRM signal handler - needed for Solaris, HP
    $SIG{ 'ALRM' } = 'alarmhandler';
}


# -----------------------------------------------------------------------------
#  SUB : get_systemc_home
#
#  Get the SYSTEMC_HOME environment variable's value.
# -----------------------------------------------------------------------------

sub get_systemc_home
{
    if( ! defined $ENV{ 'SYSTEMC_HOME' } ) {
        &print_log( "Error: " .
                   "environment variable SYSTEMC_HOME is not defined!\n" );
        exit 1;
    }

    my $sysc_home = $ENV{ 'SYSTEMC_HOME' };
    $sysc_home=~s(\\)(/)g;  #replace any backslash with forward slash
    $sysc_home;
}


# -----------------------------------------------------------------------------
#  SUB : get_systemc_arch
#
#  Get the target architecture.
# -----------------------------------------------------------------------------

sub get_systemc_arch
{
    local( $uname_s ) = `uname -s`;
    local( $uname_r ) = `uname -r`;
    local( $arch );
    local( $cxx );
    local( $cxx_comp );

    chop( $uname_s );
    chop( $uname_r );

    if( defined $ENV{ 'CXX' } ) {
        $cxx = $ENV{ 'CXX' };
    } else {
        $cxx = "g++";
    }
    chop( $cxx_comp = `basename $cxx` );

    if( $uname_s eq "SunOS" ) {
        if( $uname_r =~ /^5/ ) {
            if( $cxx_comp eq "CC" ) {
                $arch = "sparcOS5";
            } elsif( $cxx_comp eq "c++" || $cxx_comp eq "g++" ) {
                $arch = "gccsparcOS5";
            } else {
                die "Error: unsupported compiler '$cxx'\n";
            }
        } else {
            die "Error: unsupported architecture '$uname_s $uname_r'\n";
        }
    } elsif( $uname_s eq "HP-UX" ) {
        if( $uname_r =~ /^B.11/ ) {
            if( $cxx_comp eq "aCC" ) {
                $arch = "hpux11";
            } elsif( $cxx_comp eq "c++" || $cxx_comp eq "g++" ) {
                $arch = "gcchpux11";
                # die "Error: unsupported compiler '$cxx'\n";
            } else {
                die "Error: unsupported compiler '$cxx'\n";
            }
        } else {
            die "Error: unsupported architecture '$uname_s $uname_r'\n";
        }
    } elsif( $uname_s eq "Darwin" ) {
        if( $cxx_comp eq "c++" || $cxx_comp eq "g++" ) {
            $arch = "macosx";
        } else {
            die "Error: unsupported compiler '$cxx'\n";
        }
    } elsif( $uname_s eq "Linux" ) {
        if( $uname_r =~ /^2/ ) {
            if( $cxx_comp eq "c++" || $cxx_comp eq "g++" ) {
                $arch = "linux";
            } else {
                die "Error: unsupported compiler '$cxx'\n";
            }
        } else {
            die "Error: unsupported architecture '$uname_s $uname_r'\n";
        }
    } elsif( $uname_s =~ /CYGWIN_NT/ ) {
        if( $uname_r =~ /^1/ ) {
            if( $cxx_comp eq "cl" ) {
                #find MSVC version
                #reassign stderr and stdout
                open SAVEOUT, ">&STDOUT";
                open SAVERR, ">&STDERR";
                open STDOUT, ">tempfile.out" or die "Can't redirect stdout";
                open STDERR, ">&STDOUT" or die "Can't dup stdout";             
                select STDERR; $| = 1;
                select STDOUT; $| = 1;
                
                qx /cl/;   #cl returns version number in stderr
                
                #restore stderr and stdout
                close STDOUT;
                close STDERR;
                open STDOUT, ">&SAVEOUT";
                open STDERR, ">&SAVERR";
                
                #get version string and delete tempfile
                open(CL_STRING, "<tempfile.out");
                $v_string = join ( "", <CL_STRING>);
                unlink "tempfile.out";
                
                if ( $v_string =~ /.+Version 12.00/ ) {
                  $arch = "msvc60";
                } 
                elsif ( $v_string =~ /.+Version 13.10/ )  {
                  $arch = "msvc71";
                }
                else {
                  die "Error: unsupported compiler '$cxx'\n";
                }
            } elsif( $cxx_comp eq "c++" || $cxx_comp eq "g++" ) {
                $arch = "cygwin";
            } else {
                die "Error: unsupported compiler '$cxx'\n";
            }
        } else {
            die "Error: unsupported architecture '$uname_s $uname_r'\n";
        }
    } else {
        die "Error: unsupported architecture '$uname_s $uname_r'\n";
    }

    $rt_cc = $cxx;
    $arch;
}


# -----------------------------------------------------------------------------
#  SUB : init_globals
#
#  Initialization routine.
# -----------------------------------------------------------------------------

sub init_globals
{
    # setup signal handlers
    $SIG{ 'INT' }  = 'interrupt_handler';
    $SIG{ 'QUIT' } = 'interrupt_handler';
    $SIG{ 'ALRM' } = 'alarm_handler';

    $rt_systemc_home = &get_systemc_home;
    $rt_systemc_arch = &get_systemc_arch;

    $rt_cleanup = 1;                    # cleanup temp dirs by default
    $rt_mail = 0;                       # send mail with results
    $rt_email_admin = "";
    $rt_email_dist = "";
    $rt_max_file_size = 500000;         # limit the max size of the output file
    chop( $rt_output_dir = `pwd` );     # directory for output logs
    $rt_prodname = "systemc.exe";       # simulation executable name
    $rt_quick_tests = 0;
    $rt_tests_dir = "$rt_systemc_test/tests";
    $rt_time_tests = 0;
    $rt_verbose  = 0;

    $rt_opts = '';                      # options to use for compile
    if( defined $ENV{ 'RT_OPTS' } ) {
        $rt_opts = $ENV{ 'RT_OPTS' };
    }
    $rt_output_file = "verify.log";     # copy output to file
    $rt_props = 0;                      # global properties (from args/config)
    $rt_diag = 0;                       # diag mode
    $rt_timeout = 0;                    # timeout in min (zero for off)
    %rt_testlist = ();

    # set the property file names
    $rt_dontrun = "DONTRUN";
    $rt_compile = "COMPILE";
    $rt_ig_retval = "IG_RETVAL";

    # file and directory permissions
    $rt_file_permissions = 0666;
    $rt_dir_permissions  = 0777;

    $ENV{ 'SYSTEMC_REGRESSION' } = 1;

    # Set compiler and compiler flags
    if( $rt_systemc_arch eq "gccsparcOS5" ) {
        $rt_ccflags = "-Wall";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O2";
    } elsif( $rt_systemc_arch eq "sparcOS5" ) {
        $rt_ccflags = "";
        $rt_ld = $rt_cc;
        $rt_ldflags = "-xildoff";
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O3";
    } elsif( $rt_systemc_arch eq "gcchpux11" ) {
        $rt_ccflags = "-Wall";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O2";
    } elsif( $rt_systemc_arch eq "hpux11" ) {
        $rt_ccflags = "-Aa -ext +DA2.0 +DS2.0";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "+O1";
    } elsif( $rt_systemc_arch eq "linux" ) {
        $rt_ccflags = "-Wall";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O2";
    } elsif( $rt_systemc_arch eq "macosx" ) {
        $rt_ccflags = "-Wall";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O3";
    } elsif( $rt_systemc_arch eq "cygwin" ) {
        $rt_ccflags = "-Wall";
        $rt_ld = $rt_cc;
        $rt_ldflags = $rt_ccflags;
        $rt_debug_flag = "-g";
        $rt_optimize_flag = "-O2";
    } elsif( $rt_systemc_arch eq "msvc60" ) {
        $rt_cc = "CL.EXE";
        $rt_ccflags = "/nologo /GR /GX /Zm800 /Op";
        ( $cc_dir ) = qx/which cl/ =~ /(.+)\/cl/;
        print "\$cc_dir = $cc_dir\n";
        $rt_ld = "\"$cc_dir/LINK.EXE\"";
        $rt_ldflags = "/nologo /NODEFAULTLIB:LIBCD";
        $rt_debug_flag = "/GZ";
        $rt_optimize_flag = "/O2";
    } elsif( $rt_systemc_arch eq "msvc71" ) {
        $rt_cc = "CL.EXE";
        $rt_ccflags = "/nologo /D \"NOGDI\" /GR /Wp64 /MLd /EHsc /RTC1 /vmg";
        ( $cc_dir ) = qx/which cl/ =~ /(.+)\/cl/;
        print "\$cc_dir = $cc_dir\n";
        $rt_ld = "\"$cc_dir/LINK.EXE\"";
        $rt_ldflags = "/nologo";
        $rt_debug_flag = "/GZ";
        $rt_optimize_flag = "/O2";
    }

    $rt_add_ldpaths = '';  # additional link paths
    if( defined $ENV{ 'RT_ADD_LDPATHS' } ) {
        $rt_add_ldpaths = $ENV{ 'RT_ADD_LDPATHS' };
    }
    $rt_add_ldlibs = '';   # additional link libraries
    if( defined $ENV{ 'RT_ADD_LDLIBS' } ) {
        $rt_add_ldlibs = $ENV{ 'RT_ADD_LDLIBS' };
    }
    $rt_add_filter = '';   # additional filter for scl_strip
    if( defined $ENV{ 'RT_ADD_FILTER' } ) {
        $rt_add_filter = $ENV{ 'RT_ADD_FILTER' };
    }

    # Defines for properties
    %rt_test_props = ( 'comp_only',  0x1,
                       'ig_retval',  0x2,
                       'debug',      0x4,    # compile with debug flag
                       'purify',     0x8,    # link with purify
                       'quantify',   0x10,   # link with quantify
                       'dontrun',    0x20,   # dir marked dontrun or 
                                             #   is not for this arch
                       'optimize',   0x40,   # compile with optimize flag
                       'purecov',    0x80    # link with purecov
                       );

    # Defines for striplog functions
    $rt_sim_pat        = '^SystemC';
    $rt_strip_head_pat = '^SystemC Simulation';
    $rt_strip_tail_pat = '^End of SystemC Simulation';

    %rt_test_type = ( 's', 'cpp',            # single test (SystemC c++)
                      'f', 'f',              # include (-f) file
                      'c', 'mk',             # c shell test
                      'i', 'in',             # input file
                      'scr', 'scr'           # script file
                      );

    # Defines the returned error codes and classifed failures
    # depending on where in run_test the error occurs
    %rt_error_code = (
        'fvvc',               '              linking              ',
        'fcomp',              '             compiling             ',
        'fsym',               '              running              ',
        'fdiff',              '                diff               ',
        'missolog',           '          missing outlog           ',
        'difffile',           '            diff file              ',
        'diff_prog_fail',     '        diff prog failed           ',
        'o_exst_stp_input',   '         missing outlog            ',
        'g_exst_stp_input',   '         missing golden            ',
        'o_copen_stp_input',  '      open outlog for strip        ',
        'g_copen_stp_input',  '      open golden for strip        ',
        'o_bad_sim_type',     '     bad/no sim name in outlog     ',
        'g_bad_sim_type',     '     bad/no sim name in golden     ',
        'o_copen_stp_output', '     open output for strip outlog  ',
        'g_copen_stp_output', '     open output for strip golden  ',
        'o_copen_stdout',     'open stdout for outlog strip output',
        'g_copen_stdout',     'open stdout for golden strip output',
        'pass',               '              passed               ',
        'skip',               '              skipped              ',
        'unknown',            '          unknown problem          '
                      );
}


# -----------------------------------------------------------------------------
#  SUB : usage
#
#  Prints the usage.
# -----------------------------------------------------------------------------

sub usage
{
    print <<"EOM";
Usage: $0 [<options>] <directories|names>

  Arguments:
    <directories>  Search for tests in the specified directories.
    <names>        Run tests using the specified names as basenames.
                   Unless the extension is also given, all tests
                   sharing the same basename will be run.
    <options>
      -no-cleanup  Do not clean up temporary files and directories.
      -f <file>    Use file to supply tests.
      -g           Compile tests with debug flag.
      -m           Send mail with results.
      -o <opts>    Additional compiler options.
      -O           Compile tests with optimize flag.
      -purecov     Link tests with purecov.
      -purify      Link tests with purify.
      -quantify    Link tests with quantify.
      -Q           Run quick tests only.
      -t <time>    Set the timeout for a test in minutes (default 5 minutes).
      -T           Measure runtime of tests in seconds.
      -v           Verbose output.

EOM
    exit 1;
}


# -----------------------------------------------------------------------------
#  SUB : parse_args
#
#  Parse the command line arguments.
# -----------------------------------------------------------------------------

sub parse_args
{
    local( @arglist ) = @_;

    # print usage if no arguments
    &usage if( $#arglist < 0 );

    local( $tests ) = '';
    local( $files ) = '';
  
    while( $#arglist >= 0 ) {

        # next argument
        local( $arg ) = shift @arglist;

        if( $arg =~ /^-/ ) {  # must be -arg
          
            # do not cleanup
            if( $arg =~ /^-no-cleanup/ ) {
                $rt_cleanup = 0;
                next;
            }
          
            # include file
            if( $arg =~ /^-f/ ) {
                $arg = shift @arglist;
                $files .= ( $files eq '' ) ? "$arg" : " $arg";
                next;
            }

            # compile with debug flag
            if( $arg =~ /^-g/ ) {
                $rt_props = $rt_props | $rt_test_props{ 'debug' };
                next;
            }

            # send mail with results
            if( $arg =~ /^-m/ ) {
                $rt_mail = 1;
                next;
            }

            # additional compiler options
            if( $arg =~ /^-o/ ) {
                $arg = shift @arglist;
                $rt_opts .= ( $rt_opts eq '' ) ? "$arg" : " $arg";
                next;
            }

            # compile with optimize flag
            if( $arg =~ /^-O/ ) {
                $rt_props = $rt_props | $rt_test_props{ 'optimize' };
                next;
            }

            # link with purecov
            if( $arg =~ /^-purecov/ ) {
                $rt_props = $rt_props | $rt_test_props{ 'purecov' };
                next;
            }

            # link with purify
            if( $arg =~ /^-purify/ ) {
                $rt_props = $rt_props | $rt_test_props{ 'purify' };
                next;
            }

            # link with quantify
            if( $arg =~ /^-quantify/ ) {
                $rt_props = $rt_props | $rt_test_props{ 'quantify' };
                next;
            }

            # run quick tests only
            if( $arg =~ /^-Q/ ) {
                $rt_quick_tests = 1;
                next;
            }
          
            # timeout value
            if( $arg =~ /^-t/ ) {
                $arg = shift @arglist;
                $rt_timeout = $arg;
                next;
            }

            # time tests
            if( $arg =~ /^-T/ ) {
                $rt_time_tests = 1;
                next;
            }

            # verbose
            if( $arg =~ /^-v/ ) {
                $rt_verbose = 1;
                next;
            }
          
            &print_log( "Error: unknown argument '$arg'\n");
            exit 1;
        }
      
        # must be test
        else {
            $tests .= ( $tests eq '' ) ? "$arg" : " $arg";
        }
    }

    # print usage if no tests specified
    &usage if( $tests eq '' ) && ( $files eq '' );

    ( $tests, $files );
}


# -----------------------------------------------------------------------------
#  SUB : print_intro
#
#  Print the intro.
# -----------------------------------------------------------------------------

sub print_intro
{
    local( $script );
    ( $script = $0 ) =~ s|^.*/||;
    local( $host );
    chop( $host = `hostname` );
    local( $uname_a );
    chop( $uname_a = `uname -a` );
    local( $date );
    chop( $date = `date` );

    &print_log( "\n*** $script started on host $host\n\n" );
    &print_log( "$uname_a\n" );
    &print_log( "$date\n" );

    &print_log( "\n*** settings\n\n" );
    &print_log( "SYSTEMC_ARCH : $rt_systemc_arch\n" );
    local( $working_view ) = '';
    local( $ct ) = "/usr/atria/bin/cleartool";
    if( -x $ct ) {
        $working_view = `$ct pwv -short`;
        chop( $working_view );
    }
    local( $vob ) = "/vobs/abc/src/scenic/scenery";
    &print_log( "SYSTEMC_HOME : " );
    if( $rt_systemc_home =~ m|^$vob| ) {
        &print_log( "[$working_view] " );
    }
    &print_log( "$rt_systemc_home\n" );
    &print_log( "SYSTEMC_TEST : " );
    if( $rt_systemc_test =~ m|^$vob| ) {
        &print_log( "[$working_view] " );
    }
    &print_log( "$rt_systemc_test\n" );
    &print_log( " OUTPUT_DIR  : " );
    if( $rt_output_dir =~ m|^$vob| ) {
        &print_log( "[$working_view] " );
    }
    &print_log( "$rt_output_dir\n" );

    &print_log( "\n*** running tests\n" );
}


# -----------------------------------------------------------------------------
#  SUB : get_props
#
#  - Find properties for directory;
#  - find directory test type (single, include, shell);
#  - return bits representing properties;
#  - return list of test files.
# -----------------------------------------------------------------------------

sub get_props
{
    local( $dir ) = @_;

    local( $props ) = 0;    # properties to return
    local( $arch ) = '';    # arch for this machine
    local( @tmp );
    local( @files );

    opendir( RT_DIR, $dir ) || die "Error: cannot open directory '$dir'\n";
    @files = readdir( RT_DIR );
    closedir( RT_DIR );
    
    # set .compile .ig_retval
    if( -e "$dir/$rt_compile" ) {
        $props = $props | $rt_test_props{ 'comp_only' };
    }
    if( -e "$dir/$rt_ig_retval" ) {
        $props = $props | $rt_test_props{ 'ig_retval' };
    }
    
    # set 'dontrun' for:
    #  - .dontrun
    #  - .only<arch> if were are not running on <arch>
    #  - .not<arch>  if were are running on <arch>
    if( -e "$dir/$rt_dontrun" ) {
        $props = $props | $rt_test_props{ 'dontrun' };
    } else {
        @tmp = grep( /^\.not/, @files );
        if( $#tmp >= 0 ) {       # found arch restrictions
            &print_log( "NYI: .not<arch> control\n" );
        }
        @tmp = grep( /^\.only/, @files );
        if( $#tmp >= 0 ) {       # found arch restrictions
            &print_log( "NYI: .only<arch> control\n" );
        }
    }
    
    # find directory type
    $dirtype = 'scr';
    @tmp = grep( /\.$rt_test_type{ 'scr' }$/, @files );
    $#tmp = -1;
    if( $#tmp < 0 ) {
        $dirtype = 'c';
        @tmp = grep( /\.$rt_test_type{ 'c' }$/, @files );
    }
    if( $#tmp < 0 ) {
        $dirtype = 'f';
        @tmp = grep( /\.$rt_test_type{ 'f' }$/, @files );
    }
    if( $#tmp < 0 ) {           # no include, so try single
        $dirtype = 's';
        @tmp = grep( /\.$rt_test_type{ 's' }$/, @files );
    }

    ( $props, $dirtype, @tmp );
}


# -----------------------------------------------------------------------------
#  SUB : add2testlist
#
#  - Find props for directory;
#  - add tests to %rt_testlist.
# -----------------------------------------------------------------------------

sub add2testlist
{
    # my( $testdir, $testprop, @tests ) = @_;
    local( $testdir ) = shift;
    local( $testprop ) = shift;
    local( @tests ) = @_;
    
    if( $rt_diag >=4 ) {
        &print_log( "DIAG: add2testlist: $testdir " . hex( $testprop ) . " " .
                   join( ' ', @tests ) . "\n" );
    }
    
    foreach $test ( @_ ) {
        $rt_testlist{ "$testdir $test" } = $testprop;
    }
}


# -----------------------------------------------------------------------------
#  SUB : get_children
#
#  Return all directories below current.
# -----------------------------------------------------------------------------

sub get_children
{
    local( $dir, $base ) = @_;

    local( @files );

    opendir( RT_DIR, "$dir/$base" ) ||
        die "Error: cannot open directory '$dir/$base'\n";
    @files = grep( ! /^\.\.?$/,
                   grep( ( -d "$dir/$base/$_" ) && ! ( -l "$dir/$base/$_" ),
                         readdir( RT_DIR ) ) );
    closedir( RT_DIR );
    
    grep( $_ = "$base/$_", @files );
}


# -----------------------------------------------------------------------------
#  SUB : find_it
#
#  - arg is test file or directory.
#  - returns '' on success.
#  - returns level of path where search failed:
#      o test root dir if first part of path fails.
#      o sub-directory(s) if first part of path is found but
#        either directories can't be found, or test doesn't
#        exist.
#  - sets properties for tests in each directory.
#  - adds found tests to %rt_testlist
# -----------------------------------------------------------------------------

sub find_it
{
    local( $testname ) = @_;

    local( @found ) = ();
    local( @searchdirs );        # root dirs in search
    local( @tempsearch );
    local( $tempdir );
    local( $toplevel );          # save level of path
    local( $temp );
    local( $testbase );          # path to test
    local( $dirtype );           # type of dir (single, include, csh)
    local( $dirprops );          # properties for dir (ie dontrun...)

    # remove trailing '/'
    $testname =~ s|/$||;

    # do levelwise search

    ( $toplevel ) = ( $testname =~ m|^([^/]+)| );
    ( $testbase = $testname ) =~ s|/[^/]+$||;
    $testbase = '' if $testbase eq $testname;

    &print_log( "\nDIAG: testname=$testname, toplevel=$toplevel, " .
               "testbase=$testbase\n" )
        if $rt_diag >= 1;

    # first try to find start of given path
    @tempsearch = ( '.' );
    while( $#found < 0 ) {
        @searchdirs = @tempsearch;
        &print_log( "\nDIAG: \$#searchdirs=$#searchdirs, " ) if $rt_diag >= 8;
        &print_log( "\$#tempsearch=$#tempsearch\n" ) if $rt_diag >= 8;
        @tempsearch = ();
        return $rt_tests_dir if $#searchdirs < 0;
        foreach $tempdir ( @searchdirs ) {
            opendir( RT_DIR, "$rt_tests_dir/$tempdir" );

            # found test directory or matching file
            if( grep( ( $_ eq $toplevel ) || ( ! -d $_ ),
                      grep( /^$toplevel/, readdir( RT_DIR ) ) ) ) {
                if( $tempdir =~ /^\.$/ ) {
                    push( @found, "" );
                } else {
                    push( @found, "$tempdir/" );
                }
            }

            closedir( RT_DIR ) && next if $#found >= 0;
            rewinddir( RT_DIR );
            foreach $temp ( grep( ! /^\.\.?$/,
                                  grep( ( -d "$rt_tests_dir/$tempdir/$_" ) &&
                                        ! ( -l "$rt_tests_dir/$tempdir/$_" ),
                                        readdir( RT_DIR ) ) ) ) {
                if( $tempdir =~ /^\.$/ ) {
                    push( @tempsearch, "$temp" );
                } else {
                    push( @tempsearch, "$tempdir/$temp" );
                }
            }
            closedir( RT_DIR );
        }
    }

    &print_log( "DIAG: \$#found=$#found, found='" .
               join( ':', @found ) . "'\n" ) if $rt_diag >= 5;

    # create list of directories found to be used if
    #   file/dir can't be found
    local( @tmp ) = @found;
    # now toplevel is final part of name
    ( $toplevel ) = ( $testname =~ m|([^/]+)$| );
    local( $retval ) = join( ' ', grep( $_ = "$rt_tests_dir/$_", @tmp ) );
    while( $#found >= 0 ) {
        &print_log( "DIAG: \$#found=$#found, found='" .
                   join( ':', @found ) . "'\n" ) if $rt_diag >= 5;

        $temp = shift @found;

        &print_log( "DIAG: $rt_tests_dir/$temp$testbase\n" ) if $rt_diag >= 4;

        # don't waste time if path can't be found
        next if ! -d "$rt_tests_dir/$temp$testbase";

        # full path to single test
        ( $_ ) = ( $testname =~ /\.([a-z]{1,3})$/ );
        if( ( -f "$rt_tests_dir/$temp$testname" ) &&
            ( -f "$rt_tests_dir/$temp${testbase}/$toplevel" ) &&
            # test for invalid test extension
            ( ( $_ =~ /$rt_test_type{ 's' }/ ) ||
              ( $_ eq $rt_test_type{ 'f' } ) ||
              ( $_ eq $rt_test_type{ 'c' } ) ) ) {

            # get directory properties
            ( $dirprops, $dirtype, @tmp ) =
                &get_props( "$rt_tests_dir/$temp$testbase" );
            $dirprops = $dirprops | $rt_props;    # add global props

            ( $_ ) = ( $testname =~ m|/?([^/]+)$| );
            $testname =~ s|/?[^/]+$||;
            $temp =~ s|/$|| if $testname eq '';
            &add2testlist( "$temp$testname", $dirprops, $_ );
            $retval = '';
        }
        else {

            # full path to directory
            if( -d "$rt_tests_dir/$temp$testname" ) {
                &print_log( "DIAG: dir='$rt_tests_dir/$temp$testname'\n" )
                    if $rt_diag >= 4;

                # check if directory is execused
                ( $dirprops, $dirtype, @tmp ) =
                    &get_props( "$rt_tests_dir/$temp$testname" );
                next if( $dirprops & $rt_test_props{ 'dontrun' } ) > 0;

                # first look for children
                @tmp = &get_children( "$rt_tests_dir", "$temp$testname" );
                if( $#tmp >= 0 ) {
                    # we found children so update @found, add 
                    #   $testname to $temp, and zero $testname
                    push( @found, @tmp );   # adding children
                    $temp .= $testname;
                    $testname = '';
                    $testbase = '';
                }

                # get directory properties
                ( $dirprops, $dirtype, @tmp ) =
                    &get_props( "$rt_tests_dir/$temp$testname" );
                @tmp = grep( -f "$rt_tests_dir/$temp${testname}/$_", @tmp );
                next if $#tmp < 0;  # no tests in dir
                next if( $dirprops & $rt_test_props{ 'dontrun' } ) > 0;
                $dirprops = $dirprops | $rt_props;    # add global props

                &add2testlist( "$temp$testname", $dirprops, @tmp );
                $retval = '';
            }

            # find matching files that begin with same name
            else {
                # get directory properties
                ( $dirprops, $dirtype, @tmp ) =
                    &get_props( "$rt_tests_dir/$temp${testbase}" );
                @tmp = grep( -f "$rt_tests_dir/$temp${testbase}/$_", @tmp );
                next if $#tmp < 0;  # no tests in dir
                next if( $dirprops & $rt_test_props{ 'dontrun' } ) > 0;
                $dirprops = $dirprops | $rt_props;    # add global props

                @tmp = grep( m|^$toplevel[^/]+$|, @tmp );
                if( $#tmp >= 0 ) {
                    $temp =~ s|/$|| if $testbase eq '';
                    &add2testlist( "$temp$testbase", $dirprops, @tmp );
                    $retval = '';
                }
            }
        }
    }
    
    return $retval;
}


# -----------------------------------------------------------------------------
#  SUB : get_testlist
#
#  - Arguments:
#    string of whitespace delimited file or directory names
#    string containing name of -f include file
#  - Puts all testnames in associative array (%rt_testlist) using the
#    testname as the key and putting attributes about the test in
#    the value.  Testname is space separated path and name:
#    "private/bugs bug21.f"
#  - Uses dot files to set attributes.
#  - Returns 1 for success, 0 for failure
# -----------------------------------------------------------------------------

sub get_testlist
{
    local( $namelist,              # list of names
           $include_names          # list of include filenames
           ) = @_;

    local( @namearray );              # array for names and filenames
    local( $listlen );                # number of names in namelist
    local( $temp );
    local( $err_file );
    local( $temp_fname );
    local( $testnames );              # tests found in search

    # process command line names
    foreach $temp ( split( / /, $namelist ) ) {
        next if $temp =~ /^\s*$/;
        $testnames = &find_it( $temp );    # returns '<paths>' on failure
        if( $testnames ne '' ) {
            # error because dir exists but has no tests
            foreach $err_file ( split( ' ', $testnames ) ) {
                if( -d "$err_file$temp" ) {
                    &print_log( "Error: test directory '$err_file$temp'\n\t" .
                               "doesn't contain any tests\n" );

                    # error because dir or tests don't exist
                } else {
                    &print_log( "Error: '$temp' not found in '$err_file'\n" );
                }
            }
            return 0;
        }
    }

    # process include files
    foreach $temp_fname ( split( /\s+/, $include_names ) ) {
        next if $temp_fname =~ /^\s*$/;
        if( ! ( -e $temp_fname ) ) {
            &print_log( "Error: '$temp_fname' does not exist\n" );
            return 0;
        }
        if( ! open( RT_INCFILE, $temp_fname ) ) {
            &print_log( "Error: '$temp_fname' cannot be opened for reading\n" );
            return 0;
        }
        while( <RT_INCFILE> ) {
            s/\n//;
            next if /^#/;
            foreach $temp ( split( /\s+/, $_ ) ) {
                next if $temp =~ /^\s*$/;
                $temp =~ s/\r//g;
                $testnames = &find_it( $temp );  # returns '<paths>' on failure
                if( $testnames ne '' ) {
                    # error because dir exists but has no tests
                    foreach $err_file ( split( ' ', $testnames ) ) {
                        if( -d "$err_file$temp" ) {
                            &print_log( "Error: test directory " .
                                       "'$err_file$temp'\n\t" .
                                       "doesn't contain any tests\n" );
                            
                            # error because dir or tests don't exist
                        } else {
                            &print_log( "Error: '$temp' not found in " .
                                       "'$err_file'\n" );
                        }
                    }
                    return 0;
                }
            }
        }
        close RT_INCFILE;
    }
    1;
}


# -----------------------------------------------------------------------------
#  SUB : create_dir
#
#  Create the given directory (if needed), and go there.
# -----------------------------------------------------------------------------

sub create_dir
{
    local( $dir ) = @_;

    # remove first / from $dir
    $dir =~ s|/||;

    # start at the root directory
    chdir( '/' );

    local( $d );
    foreach $d ( split( '/', $dir ) ) {
        # create directory $d if it doesn't exist
        if( ! ( -d "$d" ) ) {
            if( ! mkdir( $d, $rt_dir_permissions ) ) {
                &print_log( "Error: cannot create directory '$d'\n" );
                # failed
                return 0;
            }
        }
        # go to directory $d
        if( ! chdir( $d ) ) {
            &print_log( "Error: cannot go to directory '$d'\n" );
            # failed
            return 0;
        }
    }

    # succeeded
    1;
}


# -----------------------------------------------------------------------------
#  SUB : setup_for_test
#
#  - cd to proper log directory
#  - create log directories as needed
#  - create symlink for source dir
#  - return last level of path to test
# -----------------------------------------------------------------------------

sub setup_for_test
{
    local( $currtestdir ) = @_;

    local( $linkdir );

    split( '/', $currtestdir );
    $linkdir = pop @_;

    # create log path and cd to it
    &create_dir( "$rt_output_dir/$currtestdir" );

    if( $rt_systemc_arch =~ /^msvc/ ) {

        # the 'cl' compiler doesn't accept links
        local( @args );
        @args = ( "cp", "-pr", "$rt_tests_dir/$currtestdir", "." );
        system( @args ) == 0
            or die "system @args failed: $?";

    } else {

        eval 'symlink( "$rt_tests_dir/$currtestdir", "$linkdir" );';

        if( ! ( -e $linkdir ) ) {
            &print_log( "Error: unable to establish symbolic link " .
                        "'$rt_tests_dir/$currtestdir' to '$linkdir'\n" );
        }

    }

    # return last part of path to test
    $linkdir;
}


# -----------------------------------------------------------------------------
#  SUB : rt_system
#
#  - run system call with signal capture
#  - any call given through here should 
#    redirect output (have meta chars)
# -----------------------------------------------------------------------------

sub rt_system
{
    local( $command ) = @_;

    local( $ret_code );
    local( $signal );
    
  FORK: {
      if( $rt_child_pid = fork ) {
          # wait for command to complete
          waitpid( $rt_child_pid, 0 );
          undef $rt_child_pid;
          $ret_code = $? >> 8;
          $signal = $? & 255;
          return ( $ret_code, $signal );

      } elsif( defined $rt_child_pid ) {
          # execute command
          # (add exec so process is command, not sh)
          exec( "exec $command" ) || exit -1;

      } elsif( $! =~ /No more process/ ) {
          sleep 5;
          redo FORK;
      } else {
          return 1;
      }
  }
}


# -----------------------------------------------------------------------------
#  SUB : file_size
#
#  - check the size of the log file
#  - return the size of the file
# -----------------------------------------------------------------------------

sub file_size
{
    local( $file ) = @_;

    local( $size ) = -s( $file );
    $size;
}


# -----------------------------------------------------------------------------
#  SUB : rt_timed_system
#
#  ******PLEASE READ COMMENTS VERY CAREFULLY BEFORE MAKING CHANGES*******
#  - run system call with timeout and checking size of output file
# -----------------------------------------------------------------------------

sub rt_timed_system
{
    local( $command, $timeout, $file ) = @_;

    local( $size );
    local( $ret_code );
    local( $signal );

  FORK: {
      # fork here to monitor the cpu time for the simulator
      if( $rt_child_pid = fork ) {
          alarm( $timeout * 60 );
          # do another fork here to monitor the size of the log file
          if( $rt_size_pid = fork ) {
              # parent process
              # does nothing
          } elsif( defined $rt_size_pid ) {
              # child process
              # if filesize > maxfile size terminate
              while( 1 ) {
                  sleep( 5 );
                  $size = &file_size( $file );
                  if( $size > $rt_max_file_size ) {
                      kill 'TERM', $rt_child_pid;
                      exit 0;
                  }
              }
          }
          # wait for termination of either child procees
          # note that this is ABSO necessary else there will be defunct 
          #   processes floating around
          # note that this is the process whose exit code we are interested
          #   in and not the size process
          waitpid( $rt_child_pid, 0 );
          $ret_code = ($? >> 8);
          $signal = ($? & 255);
          # now kill the size process and wait for it to terminate
          #   else there will be defunct processes floating around
          kill 'TERM', $rt_size_pid;
          waitpid( $rt_size_pid, 0 );
          alarm 0;
          undef $rt_size_pid;
          undef $rt_child_pid;
          return( $ret_code, $signal );

      } elsif( defined $rt_child_pid ) {
          # execute command
          # (add exec so process is command, not sh)
          exec( "exec $command" ) || exit -1;
      } elsif( $! =~ /No more process/ ) {
          sleep 5;
          redo FORK;
      } else {
          return 1;
      }
  }
}


# -----------------------------------------------------------------------------
#  SUB : compile_files
#
#  Compile set of files.
# -----------------------------------------------------------------------------

sub compile_files
{
    local( $filename, $testname, $command ) = @_;

    local( $temp );
    local( $exit_code );
    local( $signal );
    local( $newcommand );
    local( $ofile );
    local( $tmp );
    local( $file );
    local( $ofilestring );
    
    open( FILE, "<$filename" ) ||
        &print_log( "Error: cannot open file '$filename'\n" );
    
    $files = "";
    $ofilestring = "";
    $first_time = 1;

    &print_log( "Compiling\n" );

    while( <FILE> ) {
        $temp = $_;                             # <link_dir>/<basename>.cpp

        chop( $temp );
        $file = `basename $temp .cpp`;          # file = <basename>
        chop( $file );

        if( $rt_systemc_arch =~ /^msvc/ ) {
            $ofile = "$file.obj";
        } else {
            $ofile = "$file.o";
        }
        $ofilestring .= " $ofile";

        $newcommand = $command;
        # if first time create the file, else append to it
        if( $first_time ) {
            $first_time = 0;
#           if( $rt_systemc_arch eq "msvc60" ) {
#               $newcommand .= " $temp /Fo $ofile 1> $testname.log  2>&1";
#           } else {
#               $newcommand .= " $temp -o $ofile 1> $testname.log  2>&1";
#           }
            $newcommand .= " $temp 1> $testname.log 2>&1";
        } else {
#           if( $rt_systemc_arch eq "msvc60" ) {
#               $newcommand .= " $temp /Fo $ofile 1>> $testname.log  2>&1";
#           } else {
#               $newcommand .= " $temp -o $ofile 1>> $testname.log  2>&1";
#           }
            $newcommand .= " $temp 1>> $testname.log 2>&1";
        }

        &print_log( "  $file\n" ) unless $rt_verbose;
        &print_log( "  $newcommand\n" ) if $rt_verbose;

        ( $exit_code, $signal ) = &rt_timed_system( $newcommand, $rt_timeout,
                                                    "$testname.log" );

        if( $exit_code != 0 || $signal != 0 ) {
            return ( $exit_code, $signal, $temp, $ofilestring );
        }
        next;
    }
    close FILE;

    return ( 0, 0, 0, $ofilestring );
}


# -----------------------------------------------------------------------------
#  SUB : strip_tracelog
#
#  Routine that strips first 7 lines from .vcd and .awif log files,
#  which contain SystemC version and date information.
# -----------------------------------------------------------------------------

sub strip_tracelog
{
    local( $log_file, $stripped_file ) = @_;

    # check existence of log file
    if( ! -e $log_file ) {
        &print_log( "Error: cannot find log file '$log_file'\n" );
        return 'exst_stp_input';
    }

    local( $dir );
    
    # put output in file $stripped_file 
    ( $dir = $stripped_file ) =~ s|/[^/]+$||;
    &create_dir( $dir );
    
    $command = "`tail +8 < $log_file | sed -e 's:\r::' > $stripped_file`";

    ( $exit_code, $signal ) = &rt_system( $command );

    # chmod( $rt_file_permissions, "$stripped_file" );

    0;
};


# -----------------------------------------------------------------------------
#  SUB : check_sim_in_log
#
#  Read as much of the log file as needed to find simulator pattern.
# -----------------------------------------------------------------------------

sub check_sim_in_log
{
    for( @strip_logfile ) {
        /$rt_sim_pat/o && return 0;
    }

    &print_log( "Error: failed to find simulator pattern!\n" );
    return 1;
}


# -----------------------------------------------------------------------------
#  SUB : strip_tail
#
#  Remove tail banner up to beginning of compile messages.
# -----------------------------------------------------------------------------

sub strip_tail
{
    local( $eval_cmds );
    
    # use eval loop so regex compiles once
    $eval_cmds = <<"EVAL";
    local( \$tmp ) = $#strip_logfile;
    while( \$tmp >= 0 ) {
        last if \$strip_logfile[\$tmp] =~ /$rt_strip_tail_pat/;
        \$tmp --;
    }
    \$#strip_logfile = \$tmp - 1 unless \$tmp < 0;
EVAL
    eval $eval_cmds;
}


# -----------------------------------------------------------------------------
#  SUB : strip_header
#
#  Remove header banner up to beginning of compile messages.
# -----------------------------------------------------------------------------

sub strip_header
{
    local( $eval_cmds );
    
    # use eval loop so regex compiles once
    $eval_cmds = <<"EVAL";
    while( \$#strip_logfile >= 0 ) {
          last if \$strip_logfile[0] =~ /$rt_strip_head_pat/;
          shift \@strip_logfile;
      }
EVAL
    eval $eval_cmds;
}


# -----------------------------------------------------------------------------
#  SUB : scl_strip
# -----------------------------------------------------------------------------

sub scl_strip
{
    while( $#strip_logfile >= 0 ) {
        ( $_ = shift( @strip_logfile ) ) =~ s|$rt_systemc_home|\$SYSTEMC_HOME|;
        # remove file and line number information (always changes!)
        s|^(In file: ).*/src/systemc/.*$|$1<removed by verify\.pl>|;
        s|^(In file: ).*/include/systemc/.*$|$1<removed by verify\.pl>|;
        s|^(In file: ).*:.*$|$1<removed by verify\.pl>|;
        # remove ^M in msvc60 generated output files
        s|\r||;
        # additional filter
        if( $rt_add_filter ne '' ) {
            s|$rt_add_filter||;
        }
        $strip_output_log .= $_;
    }   
}


# -----------------------------------------------------------------------------
#  SUB : strip_log
#
#  Strip simulator specific info from log file;
#  Filename passed to striplog is expected
#    to already be verified (full rooted
#    pathname must be given).
#
# -------- Significant Modifications:  Dec 27, 1996, JLMurdock-------------
# Arguments: (1) $logfilename must be full rooted pathname for file
#                     to be stripped.
#
#            (2) $outfilename can be either a full rooted pathname of
#                     where to put the output,  or it can be nothing.
#                     If no second argument is given, then output goes
#                     to stdout.
#
# Return values: one of {0, exst_stp_input, copen_stp_input, bad_sim_type,
#                        copen_stp_output, copen_stdout}
#
#                           0:  means everything went OK.
#              exst_stp_input:  means the input file for stipping, i.e. 
#                               $logfilename, doesn't exist.
#             copen_stp_input:  means "Can't open the input file, 
#                               $logfilename, for stripping."
#            copen_stp_output:  means "Can't open the file, $outfilename, to
#                               recieve the output of the strip."
#                bad_sim_type:  means the type of a recognized simulator,
#                               (e.g. Vivace, OVIsim, VerilogXL) could not
#                               be found in $logfilename.  This can also
#                               mean that the simulation failed horribly
#                               (e.g. a seg fault in Vivace) and the 
#                               simulator's name never made it into the log.
#                               Stripping cannot be done unless the name of
#                               the simulator (i.e. sim_type) is known.
#                copen_stdout:  means that the striplog subroutine attempted
#                               to write to STDOUT, but was unsuccessful.  
#                               (striplog writes to STDOUT when no 
#                               $outfilename is given.)
# -----------------------------------------------------------------------------

sub strip_log
{
    local( $logfilename, $outfilename ) = @_;

    local( $routine );
    local( $path );
    
    @strip_logfile = ();        # use these as globals within
    $strip_output_log = '';     # the strip routines...
    
    if( ! ( -e $logfilename ) ) {
        &print_log( "Error: file to be stripped, '$logfilename', " .
                   "does not exist\n" );
        return 'exst_stp_input';
    }

    if( ! open( STRIP_IN, "$logfilename" ) ) {
        &print_log( "Error: cannot open log file to be stripped " .
                   "'$logfilename'\n" );
        return 'copen_stp_input';
    }
   
    @strip_logfile = <STRIP_IN>;

    # if segfault while simulating
    if( &check_sim_in_log ) {
        return 'bad_sim_type';
    }

    &strip_tail;

    &strip_header;
    
    $routine = "&scl_strip;";
    eval $routine; # Also apparently using globals.
    
    if( $outfilename eq '' ) { # stripped log goes to stdout
        if( ! open( STRIP_OUT, ">-" ) ) {
            &print_log( "Error: cannot dup stdout\n" );
            return 'copen_stdout';
        }
    } else { # put output in file $outfilename
        ( $path = $outfilename ) =~ s|/[^/]+$||;
        &create_dir( $path );
        if( ! open( STRIP_OUT, ">$outfilename" ) ) {
            &print_log( "Error: cannot open stripped output file " .
                       "'$outfilename'\n" );
            return 'copen_stp_output';
        }
    }
    # chmod( $rt_file_permissions, "$outfilename" );
    print STRIP_OUT $strip_output_log;
    close STRIP_OUT;

    0; # Return 0 if everything went OK.
}


# -----------------------------------------------------------------------------
#  SUB : diff_log
#
#  Diff the log file and the golden reference file.
# -----------------------------------------------------------------------------

sub diff_log
{
    local( $logfile ) = @_;

    # strip log file
    local( $striplogfile ) = $logfile . ".stripped";
    local( $new ) = "$rt_output_dir/$striplogfile";
    local( $result );
    # use strip_tracelog() for trace tests, striplog() for other tests
    if( $logfile =~ /vcd_trace/ || $logfile =~ /wif_trace/ ) {
        $result =  &strip_tracelog( "$rt_output_dir/$logfile", $new );
    } else {
        $result =  &strip_log( "$rt_output_dir/$logfile", $new );
    }
    if( $result ) {
        return "o_$result";
    }

    # check for golden log file
    local( $golden_logfile );
    # first: platform dependent file
    ( $golden_logfile = $logfile ) =~
        s|^(.*)/([^/]+)$|$1/golden/$2\.$rt_systemc_arch|;
    if( ! -e "$rt_tests_dir/$golden_logfile" ) {
        # second: platform independent file
        ( $golden_logfile = $logfile ) =~ s|^(.*)/([^/]+)$|$1/golden/$2|;
        if( ! -e "$rt_tests_dir/$golden_logfile" ) {
            &print_log( "Error: " .
                       "cannot find golden log file '$golden_logfile'\n" );
            return 'g_exst_stp_input';
        }
    }
    local( $ref ) = "$rt_tests_dir/$golden_logfile";

    # do diff
    if( ! open( DIFFCMD_OUT, "diff $new $ref |" ) ) {
        &print_log( "Error: diff on '$new' and '$ref' failed\n" );
        return 'diff_prog_fail';
    }
    local( @diff_output );
    @diff_output = <DIFFCMD_OUT>;
    close DIFFCMD_OUT;

    # passed?
    if( $#diff_output < 0 ) {
        return '';
    }

#    @diff_output = grep( /^[<>] /, @diff_output );
#    unshift( @diff_output, "< $rt_output_dir/$striplogfile\n" );
#    unshift( @diff_output, "> $rt_tests_dir/$golden_logfile\n" );
    join( '', @diff_output );
}


# -----------------------------------------------------------------------------
#  SUB : do_diff
#
#  Does strip and diff of log files.
# -----------------------------------------------------------------------------

sub do_diff
{
    local( $file ) = @_;

    local( $diff_outfile );
    local( $path );

    ( $diff_outfile = "$rt_output_dir/$file" ) =~ s|\.\w+$|.diff|;
    
    ( $path = $diff_outfile ) =~ s|/[^/]+$||;
    &create_dir( $path );

    # For trace tests diff .vcd or .awif files.
    # For all other tests diff .log files. -- Amit 3/8/99

    $file =~ tr| |/|; # combine path & name
    
    if( $file =~ /vcd_trace/ ) {
        ( $logfile = $file ) =~ s/\.\w+$/.vcd/; # change test to .vcd
    } elsif( $file =~ /wif_trace/ ) {
        ( $logfile = $file ) =~ s/\.\w+$/.awif/; # change test to .vcd
    } else {
        ( $logfile = $file ) =~ s/\.\w+$/.log/; # change test to log
    }

    if( ! ( -e "$rt_output_dir/$logfile" ) ) {
        &print_log( "Error: cannot find logfile '$logfile'\n" );
        return 'missolog';
    }
   
    $diff_result = &diff_log( $logfile );

    if( $rt_error_code{ $diff_result } ) {
        &print_log( "FAILED\n" );
        return $diff_result;
    } elsif( $diff_result ) {
        if( ! open( DIFF_OUT, ">$diff_outfile" ) ) {
            &print_log( "Error: cannot open output file '$diff_outfile'\n" );
            return 'difffile';
        }
        # chmod( $rt_file_permissions, "$diff_outfile" );
        print DIFF_OUT '-' x 79, "\n",
            "  diff failed on '$logfile'\n",
            '-' x 79, "\n";
        print DIFF_OUT $diff_result;
        close DIFF_OUT;
        &print_log( "FAILED\n" );
        return 'fdiff';
    } else { # diff produced nothing
        &print_log( "PASSED\n" );
        return 'pass';
    }
}


# -----------------------------------------------------------------------------
#  SUB : run_test
#
#  - first arg is testname without path
#  - second arg is path to test starting from testdir
# -----------------------------------------------------------------------------

sub run_test
{
    local( $currtestdir,       # path to tests
           $testname
           ) = @_;

    local( $props ) = $rt_testlist{ "$currtestdir $testname" };

    local( $type );             # test type (t=pli, s=single, f=include, c=csh)
    local( $temp );
    local( $command );          # command given to system call
    local( $linkdir );          # final dir in path to test
    local( $opts );             # command line options passed to compiler
    local( $basename );         # basename for test
    local( $exit_code );        # exit code returned by system calls
    local( $signal );           # signal on which system call terminated
    local( $tmp );
    local( $diff_result );      # value returned by calling do_diff, which 
                                # lives in rt_comm.pl.  It is a value from 
                                # the array %rt_error_code.
    local( $comp_opts );        # options to compiler (vvc)
    local( $sym_opts );         # options to simulator (sym)
    local( $err_file );
    local( $scl_include );

    &print_log( "\n" );


    # ***** Basic Setup *****

    # determine test type
    $testname =~ s/\.([a-z]{1,3})$//;
    $type = $1;
    
    # making a global to keep track of progress
    $rt_current_test = "$currtestdir/$testname.$type";

    # check for compiler
    # include your compiler check in here

    # cd and create symlink
    $linkdir = &setup_for_test( $currtestdir );
    
    # extract base name
    $basename = "$linkdir/$testname";
    
    # read options file
    $opts = "$rt_opts";

    $command = "";

    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );
    local( $num_tests ) = scalar keys %rt_testlist;
    &print_log( " TEST : $rt_current_test  ($rt_test_number/$num_tests)\n" );
    &print_log( "---------------------------------------" .
               "----------------------------------------\n" );

    local( $extra_flags ) = "";
    if( $rt_props & $rt_test_props{ 'debug' } ) {
        $extra_flags .= " $rt_debug_flag";
    }
    if( $rt_props & $rt_test_props{ 'optimize' } ) {
        $extra_flags .= " $rt_optimize_flag";
    }

    local( $pure ) = "";
    local( $pure_log );
    ( $pure_log = $rt_prodname ) =~ s|.exe|.purelog|;
    if( $rt_props & $rt_test_props{ 'purecov' } ) {
        $pure = "purecov -log-file=$pure_log";
    } elsif( $rt_props & $rt_test_props{ 'purify' } ) {
        $pure = "purify -windows=no -log-file=$pure_log";
    } elsif( $rt_props & $rt_test_props{ 'quantify' } ) {
        $pure = "quantify -windows=no -log-file=$pure_log";
    }

    # ***** COMPILE *****
    
    #
    # create compile command line for single .cpp file
    #
    if( $type =~ /$rt_test_type{'s'}/ ) {

        # compile command
        if( $rt_systemc_arch =~ /^msvc/ ) {
            $command  = "$rt_cc $rt_ccflags $extra_flags ";
            $command .= "/I. /I$rt_systemc_home/src ";
            $command .= "/c ";
        } else {
            $command  = "$rt_cc $rt_ccflags $extra_flags ";
            $command .= "-I. -I$rt_systemc_home/include ";
            $command .= "-c ";
        }

        # add user provided options to command
        $command .= " $opts";
   
        # add C++ file to command
        $command .= " ./$basename.$type";

        # add logfile to command
        $command .= " 1> $testname.log  2>&1";

        if( $rt_systemc_arch =~ /^msvc/ ) {
            # translate Cygwin path to Windows path
            $command =~ s|/cygdrive/(.)/|$1:/|;
        }

        &print_log( "Compiling\n" );
        &print_log( "  $command\n" ) if $rt_verbose;

        # run command
        ( $exit_code, $signal ) = &rt_system( $command );
    
        # error checking
        if( $exit_code != 0 || $signal != 0 ) {
            &print_log( "FAILED\n" );
            return $rt_error_code{'fcomp'};
        }

        # stop here if compile only
        if( ( $props & $rt_test_props{'comp_only'} ) > 0 ) {
            &print_log( "PASSED\n" );
            return $rt_error_code{'pass'};
        }

        # link command
        if( $rt_systemc_arch eq "msvc60" ) {
            $command  = "$rt_ld $rt_ldflags $extra_flags ";
            $command .= "/out:$rt_prodname ";
            $command .= "$testname.obj ";
            if( $rt_props & $rt_test_props{ 'debug' } ) {
              $command .= "$rt_systemc_home/msvc60/systemc/Debug/systemc.lib ";
            }
            else { 
              $command .= "$rt_systemc_home/msvc60/systemc/Release/systemc.lib ";
            }
        } elsif( $rt_systemc_arch eq "msvc71" ) {
            $command  = "$rt_ld $rt_ldflags $extra_flags ";
            $command .= "/out:$rt_prodname ";
            $command .= "$testname.obj ";
            if( $rt_props & $rt_test_props{ 'debug' } ) {
              $command .= "$rt_systemc_home/msvc71/systemc/Debug/systemc.lib ";
            }
            else {
              $command .= "$rt_systemc_home/msvc71/systemc/Release/systemc.lib ";
            }
        } else {
            $command  = "$pure $rt_ld $rt_ldflags $extra_flags ";
            $command .= "-o $rt_prodname ";
            $command .= "$testname.o ";
            $command .= "-L. -L$rt_systemc_home/lib-$rt_systemc_arch ";
            if( $rt_add_ldpaths ne '' ) {
                $command .= "$rt_add_ldpaths ";
            }
            $command .= "-lsystemc ";
            if( $rt_add_ldlibs ne '' ) {
                $command .= "$rt_add_ldlibs ";
            }
        }

        if( $rt_systemc_arch !~ /msvc/ ) {
            # add user provided options to command
            $command .= " $opts";
        }
        
        # add logfile to command
        $command .= " 1>> $testname.log  2>&1";

        if( $rt_systemc_arch eq "msvc60" ) {
            # translate Cygwin path to Windows path
            $command =~ s|/cygdrive/(.)/|$1:/|;
        }

        &print_log( "Linking\n" );
        &print_log( "  $command\n" ) if $rt_verbose;
    }

    #
    # create compile command line for command file with .f extension 
    #
    if( $type =~ /$rt_test_type{'f'}/ ) {

        # compile command
        if( $rt_systemc_arch =~ /msvc/ ) {
            $command  = "$rt_cc $rt_ccflags $extra_flags ";
            $command .= "/I. /I$rt_systemc_home/src ";
            $command .= "/c ";
        } else {
            $command  = "$rt_cc $rt_ccflags $extra_flags ";
            $command .= "-I. -I$rt_systemc_home/include ";
            $command .= "-c ";
        }

        # add user provided options to command
        $command .= " $opts";

        if( $rt_systemc_arch !~ /msvc/ ) {
            # translate Cygwin path to Windows path
            $command =~ s|/cygdrive/(.)/|$1:/|;
        }

        # call compile_files to execute command for each C++ file 
        ( $exit_code, $signal, $err_filename, $ofiles ) =
            &compile_files( "./$basename.$type", $testname, $command );

        # error checking on compile_files
        if( $exit_code != 0 || $signal != 0 ) {
            &print_log( "FAILED\n" );
            return $rt_error_code{'fcomp'};
        }

        # stop here if compile only
        if( ( $props & $rt_test_props{'comp_only'} ) > 0 ) {
            &print_log( "PASSED\n" );
            return $rt_error_code{'pass'};
        }

        # link command
        if( $rt_systemc_arch eq "msvc60" ) {
            $command  = "$rt_ld $rt_ldflags $extra_flags ";
            $command .= "/out:$rt_prodname ";
            $command .= "$ofiles ";
             $command .= "$rt_systemc_home/msvc60/systemc/Debug/systemc.lib ";
            #$command .= "$rt_systemc_home/msvc60/systemc/Release/systemc.lib ";
        } elsif( $rt_systemc_arch eq "msvc71" ) {
            $command  = "$rt_ld $rt_ldflags $extra_flags ";
            $command .= "/out:$rt_prodname ";
            $command .= "$ofiles ";
             $command .= "$rt_systemc_home/msvc71/systemc/Debug/systemc.lib ";
            #$command .= "$rt_systemc_home/msvc71/systemc/Release/systemc.lib ";
        } else {
            $command  = "$pure $rt_ld $rt_ldflags $extra_flags ";
            $command .= "-o $rt_prodname ";
            $command .= "$ofiles ";
            $command .= "-L. -L$rt_systemc_home/lib-$rt_systemc_arch ";
            if( $rt_add_ldpaths ne '' ) {
                $command .= "$rt_add_ldpaths ";
            }
            $command .= "-lsystemc ";
            if( $rt_add_ldlibs ne '' ) {
                $command .= "$rt_add_ldlibs ";
            }
        }

        if( $rt_systemc_arch !~ /msvc/ ) {
            # add user provided options to command
            $command .= " $opts";
        }

        # add logfile to command
        $command .= " 1>> $testname.log  2>&1";

        if( $rt_systemc_arch =~ /msvc/ ) {
            # translate Cygwin path to Windows path
            $command =~ s|/cygdrive/(.)/|$1:/|;
        }

        &print_log( "Linking\n" );
        &print_log( "  $command\n" ) if $rt_verbose;
    }

    #
    # create compile command line for makefile with .mk extension 
    #
    if( $type =~ /$rt_test_type{'c'}/ ) {

        $command = "make -f ./$basename.$type ";
        $command .= "CC=\"$rt_cc\" CCFLAGS=\"$rt_ccflags\" ";
        $command .= "LDFLAGS=\"$rt_ldflags\" SYSTEMC_HOME=$rt_systemc_home ";
        $command .= "SYSTEMC_ARCH=$rt_systemc_arch";
    
        # add user provided options to command
        $command .= " $opts";

        # add logfile to command
        $command .= " 1> $testname.log  2>&1";

        # stop here if compile only (???)
        if( $props & $rt_test_props{'comp_only'} > 0 ) {
            return $rt_error_code{'skip'};
        }

        &print_log( "Making\n" );
        &print_log( "  $command\n" ) if $rt_verbose;
    }
    
    # run command
    ( $exit_code, $signal ) = &rt_system( $command );


    # ***** COMPILE ERROR CHECKING *****

    if( $exit_code != 0 || $signal != 0 ) {
        &print_log( "FAILED\n" );
        return $rt_error_code{'fvvc'};
    }


    # ***** END ERROR CHECKING *****

    # stop here if compile only
    if( ( $props & $rt_test_props{'comp_only'} ) > 0 ) {
        &print_log( "PASSED\n" );
        return $rt_error_code{'pass'};
    }


    # ***** SIMULATE *****

    # run simulation
    $command = "./$rt_prodname";
    $command .= " $sym_opts";

    # add logfile
    $command .= " 1>> $testname.log  2>&1";

    &print_log( "Running\n" );
    &print_log( "  $command\n" ) if $rt_verbose;

    # time tests, before
    local( @before );
    if( $rt_time_tests ) {
        @before = times;
    }

    # running simulation without timeout.
    ( $exit_code, $signal ) = &rt_system( $command );
    if( $signal != 0 ) {
        &print_log( "FAILED\n" );
        return $rt_error_code{'fsym'};
    }

    # time tests, after
    if( $rt_time_tests ) {
        local( @after ) = times;
        &print_log( "  " . int( $after[2] - $before[2] ) . " s\n" );
    }

    $command = "echo End of SystemC Simulation";
    $command .= " 1>> $testname.log  2>&1";
    ( $exit_code, $signal ) = &rt_system( $command );
 
    # chmod( $rt_file_permissions,
    #      "$rt_output_dir/$currtestdir/$testname.log" ); # (???)

    # ***** DIFF LOGS *****

    # do diffing of the logs
    &print_log( "Comparing\n" );
   
    $diff_result = &do_diff( "$currtestdir/$testname.$type" );
    return $rt_error_code{ $diff_result };
}


# -----------------------------------------------------------------------------
#  SUB : clean_up
#
#  Remove the current test directory, including any files.
# -----------------------------------------------------------------------------

sub clean_up
{
    local( $dir );
    ( $dir = $rt_current_test ) =~ s|/[^/]+$||;
    local( $full_dir ) = "$rt_output_dir/$dir";

    opendir( RT_DIR, $full_dir ) || return 0;
    local( @files );
    @files = readdir( RT_DIR );
    closedir( RT_DIR );

    local( $f );
    foreach $f ( @files ) {
        if( -d "$full_dir/$f" && $f !~ /^\.\.?$/ ) {
            local( @args );
            @args = ( "rm", "-rf", "$full_dir/$f" );
            system( @args );
        }
        if( $f !~ /^\.\.?$/ &&
            $f !~ /.pcv$/   &&
            $f !~ /.qv$/    &&
            $f !~ /.purelog$/ ) {
            unlink "$full_dir/$f" || return 0;
        }
    }

    if( $rt_systemc_arch !~ /msvc/ ) {
        rmdir $full_dir || return 0;
    }

    1;
}


# -----------------------------------------------------------------------------
#  SUB : main
#
#  The main routine.
# -----------------------------------------------------------------------------

sub main
{
    &init_globals;

    # parse the command line arguments
    local( $tests );
    local( $files );
    ( $tests, $files ) = &parse_args( @ARGV );

    &print_intro;

    &get_testlist( $tests, $files ) || exit 1;

    local( $t );
    $rt_test_number = 0;
    foreach $t ( sort( keys %rt_testlist ) ) {
        $rt_test_number ++;
        $error_code = &run_test( split( ' ', $t ) );
        $t =~ s| |/|;
        if( $error_code eq $rt_error_code{ 'pass' } ) {
            push( @rt_pass, $t );
            &clean_up if( $rt_cleanup );
        } else {
            push( @rt_fail, "$error_code : $t" );
        }
        &print_log( "(", $#rt_pass + 1, ":", $#rt_fail + 1, ")\n" );
    }

    &compile_results;
}


# Taf!

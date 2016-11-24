#!/usr/bin/perl

# This is a simple perl script that simulates the submission of the following form:
#
# <html>
#   <body>
#     <form enctype="multipart/form-data"
#           action="http://$servername:$portnum/$script" method="post">
#       <p>Name: <input type="input" name="name" /></p>
#       <p>File: <input type="file" name="filename" /></p>
#       <p>      <input type="submit" value="Upload" /></p>
#     </form>
#   </body>
# </html>

require LWP::UserAgent;

# Server name
my $servername = "http://cse202";
# Port
my $portnum    = "42021";
# Script on server

my $script     = "s1.py";
# File to send
my $sendfile   = "submission.tar";

# create the tar to submit
#`tar czvf turnin.tgz *.c *.cc *.h Makefile`;

# set up a useragent
my $ua = LWP::UserAgent->new;
$ua->timeout(10);
$ua->env_proxy;
my $username = getpwuid($ <);

# post a request
my $response = $ua->post($servername . ":" . $portnum . '/' . $script,
						Content_Type => 'form-data',
						Content => [name => "$username",
							filename => [$sendfile],]);

# display result
if ($response->is_success) {
    print "[OK] File '$sendfile' submitted by $username\n";
}
else {
    die $response->status_line;
}

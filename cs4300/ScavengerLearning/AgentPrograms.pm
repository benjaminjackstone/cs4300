#
#
#
our $aps =
[
 {
   name => 'Snorlax',
   key  => 's',
   header => 'Snorlax.h',
   cpp    => ['Snorlax.cpp','State.cpp','cell.cpp','model.cpp', 'problem.cpp'],
   constructor => 'bjs::Scavenger::Snorlax(opts)',
 },
 {
   name => 'Manual',
   key  => 'm',
   header => 'Manual.h',
   cpp    => ['Manual.cpp'],
   constructor => 'bjs::Scavenger::Manual()',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;

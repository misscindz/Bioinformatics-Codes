#! /usr/bin/perl
use strict;
use warnings;

use Cwd ();
use Data::Dumper ();
use File::Spec ();

my ($debug) = (0);

sub transformInputFile($$$\%);
sub transformInputLine($$$\%);

my %initialReplacements;
my %stdTypeReplacements =
    (
     'LEN' => [ '8', '16', '32', '64', '-int' ],
     'bitSize' =>  [ '8', '16', '32', '64', '32' ],
     'ValueType' =>  [ qw(uint8_t uint16_t uint32_t uint64_t) ],
     'AccumType' => [ 'GtUword', 'GtUword', 'GtUword', 'GtUint64' ],
     'uOpType' => [ qw(uint8_t uint16_t uint32_t uint64_t unsigned) ],
     'iOpType' => [ qw(int8_t int16_t int32_t int64_t int) ],
     'uTypeTag' => [ 'UInt8', 'UInt16', 'UInt32', 'UInt64', 'UInt' ],
     'iTypeTag' => [ 'Int8', 'Int16', 'Int32', 'Int64', 'Int' ],
     'uOpPRI' => [ 'PRIu8', 'PRIu16', 'PRIu32', 'PRIu64', '"u"' ],
     'dOpPRI' => [ 'PRId8', 'PRId16', 'PRId32', 'PRId64', '"d"' ]
    );

my ($lengthTag, @inputs) = @ARGV;
{
  my $i;
  my $lengthTags = $stdTypeReplacements{'LEN'};
  for($i = 0; $i < @$lengthTags; ++$i)
  {
    last if($lengthTags->[$i] eq $lengthTag);
  }
  die('desired type/length tag not found') if($i >= @$lengthTags);
  foreach my $tag (keys %stdTypeReplacements)
  {
    $initialReplacements{$tag} = $stdTypeReplacements{$tag}[$i];
  }
}
my @autogenheader = <DATA>;

foreach my $input (@inputs)
{
  my ($output, $outputfh) = $input;
  $output =~ s/\.template$/$lengthTag\.c/;
  die('Failed output name generation: ', $input, ' => ', $output, "\n")
      if ($output eq $input);
  open($outputfh, '>', $output)
      or die('Failed to open ', $output, ' for writing: ');
  print $outputfh @autogenheader;
  transformInputFile($input, File::Spec->curdir(), $outputfh,
                     %initialReplacements);
  close($outputfh);
}

sub replace($\%)
{
  local $_ = $_[0];
  my $replacements = $_[1];
  exists($replacements->{$_})?$replacements->{$1}:"\@$_\@"
}


sub transformInputLine($$$\%)
{
  local $_ = shift @_;
  my ($currentDirPrefix, $outputfh, $replacements) = @_;
  if (my @matches = /^(.*?)\@include "([^"]*)"([^\@]*)\@(.*)$/)
  {
#     print(STDERR 'Found inclusion: ', $_,
#           "\n", Data::Dumper::Dumper(\@matches));
    transformInputLine($matches[0], $currentDirPrefix, $outputfh,
                       %$replacements);
    {
      my %fileLocalReplacements = %$replacements;
      my $localVarAssignments = $matches[2];
      while ($localVarAssignments =~ /\s+(\w+)=([^"]+|"[^"]*")/g)
      {
        my $substVal = $2;
        $substVal = substr($substVal, 1, -1)
            if (substr($substVal, 0, 1) eq '"'
                and substr($substVal, -1, 1) eq '"');
        $fileLocalReplacements{$1} = $substVal;
      }
      print(STDERR 'Localized replacement: ',
            Data::Dumper->Dump([\%fileLocalReplacements],
                               [qw(fileLocalReplacements)])) if $debug;
      transformInputFile($matches[1], $currentDirPrefix,
                         $outputfh, %fileLocalReplacements);
    }
    transformInputLine($matches[3], $currentDirPrefix, $outputfh,
                       %$replacements);
  }
  else # simple static pattern replacement
  {
    s/\@(\w+)\@/replace($1, %$replacements)/ge;
    print($outputfh $_);
  }
}

sub transformInputFile($$$\%)
{
  local $_;
  my ($input, $currentDirPrefix, $outputfh, $replacements) = @_;
  my ($inputfh, $currentDirHandle, $nextDirPrefix);
  my ($inputVolume, $inputDirectory, $inputFile) =
      File::Spec->splitpath($input);
  opendir($currentDirHandle, File::Spec->curdir()) or die;
  $nextDirPrefix =
      File::Spec->catpath($inputVolume, $inputDirectory, undef);
  chdir($currentDirPrefix) or die;
  open($inputfh, '<', $input)
      or die('Failed to open ', $input, ' for reading: ');
  while(<$inputfh>)
  {
    transformInputLine($_, $nextDirPrefix, $outputfh, %$replacements);
  }
  chdir($currentDirHandle) or die;
  closedir($currentDirHandle) or die;
}


__DATA__
/*
** autogenerated content - DO NOT EDIT
*/

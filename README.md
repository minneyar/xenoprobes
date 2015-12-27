XenoProbes
==========

This program is a utility for the game **Xenoblade Chronicles X**, for the Wii
U. In the game, the player is required to install data probes that extract
resources from certain sites. There are various elements that change the output
of resources, so I wrote this program to automatically search for the optimal
configuration.


Usage
-----

1. Copy the *sample-inventory.csv* file into *inventory.csv*. Open it up on a
   text editor, and change its contents to match your in-game inventory.

2. Did you discover all of Mira? With all the Sightseeing Spots? If not, open
   up *sites.csv* in a text editor and update it to match your map. If
   you haven't discovered a site, delete (or comment out) the line. If you
   don't have all Sightseeing Spots for that site yet, change the number too.

3. In a command prompt/shell, from the top directory, run:

        ./xenoprobes

    or

        xenoprobes.exe

  And be patient. At any point you can press CTRL+C to make the program stop
  early. When it finishes, it'll print out the best configuration found, and
  the expected mining/revenue/storage total (minus some rounding errors.) Try
  running the program multiple times too, each time you might get a different
  (possibly better) solution.
  
  **Hint**: if you see the program getting stuck at some value, run another
  instance in another command prompt. If the new instance reaches a better value
  before the old one gets unstuck, kill the older one. That's the nature of
  randomness.


Program Details
---------------

Try:

    ./xenoprobes --help

or

    xenoprobes.exe --help

XenoProbes has both a Hill Climbing and a Genetic Algorithm implementation.
The Hill Climbing approach is used by default, but you can try out the GA by
passing in the ``--ga`` option. I couldn't make it beat the Hill Climbing
method.

But the options you should really try to tweak are:

* ``--storageweight``, ``--revenueweight`` and ``--productionweight``: those
  affect how the program finds solutions; by default, it gives priority to
  storage (*storageweight* = 1000), then revenue (*revenueweight* = 10), and
  lastly Miranium production (*productionweight* = 1). If you want revenue to
  be more important, change the weights so *revenueweight* is larger than the
  others. Most people will want them in this order though.

* ``--iterations``: you can make this really big, and leave it running
  overnight. Then come in the morning and just press CTRL+C to stop it.

* ``--population`` and ``--offsprings``: affect how many solutions are being
  tracked at any given time, and how many variations will be generated from
  each. The larger those numbers, the more likely you'll find a good solution,
  but it slows things down.

* ``--mutation``: it's the probability that a given probe will be swapped by
  another. This shouldn't be too high, or else whenever a mutation improves a
  solution, another one might happen that ruins it. It shouldn't be too low
  either, because sometimes many swaps are needed at once to improve a solution.
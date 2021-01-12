#pragma once

#include <vector>
#include <string>
#include <tuple>

namespace neuro 
{
// consider using this one
//    using Gene = std::tuple< std::string, std::string, std::string >;
//instead of this
    class Gene 
    {
        const std::string Axon, Dendron, function;
    }

    class 

    class Genepool
    {
        std::vector< Gene > genes;
    }
}

/*

Raw genome:

A,D,f/w,a,b

Genepool:
genes:
A,D,[f]

genotype:
#,a/w,b


translation from raw to genotype
# - line number from zero

#) A,D,f/w,a,b

If f/w starting not with digit - it's a neuron:
gene:
A,D,f
genotype:
#,a,b

Else - it's a synapppppsssss
gene:
A,D
genotype
#,w

THE BEST IDEA OF MINE:

How the fuck in this silicon-based world synthetiser will understand in which layer what it should place?

We have in genotype this shit:
#,a,b
So, last field is not empty. As we've got a neuron.
Going to genes[#]:
A,D,f

And now the magic of synthesis starts. Or in Axon, or in Dendron, or in both we will find number of some gene.
And now we need to find that gene implementation in our network. Find it! Okay, if not - gene may be left as orphan... or just not be implemented
So, we've found an implementation, what's next? If it's not a synaps - mutagen fucked up (see Collision cases). OK, let it be synaps.
This synaps must have A and D. In the same manner we searching for implemetations and see that A-neuron is on layer N, and D-neuron is on layer M.
Now it's time to recall, where in the gene synaps was stated?

0) In A - wow, okay, we will place the new neuron just above A-neuron - on layer N+1 (read to the end, this rule is not for recurrent connection)
1) In D - ... obviously, under D-neuron, on layer M-1. (this also will work differently)
2) Stated in A & D - in common case, when M and N are not the same and even not M-1=N (that means we have some extra layer between N and M) - we will place in on N+1...
But hard case - we have no space! N=M-1. Solution - inserting extra layer between N and M. All the structure of NN is saved.
3) SUPER HARDCORE CASE - N=M - oh, that sucks. But that also means that it was recurrency between A-neuron and D-neuron. Let's keep this beautiful tradition
and shove this at their layer. Recurrency chain got a new step! Cheers!
4) 0xD34DB33F 0xC45E - recurrency makes brrr and suddenly M<N. Yep, interlayer recurrency, who the fuck made this net? Evolution?..
So now rules are upside down, welcome to new world. Now we will place new neuron on N-1, on M+1 and if there is no place - will push a new layer between N and M.
What about first 2 rules. In this new miraclous world of recurrency they will also be reversed. 
So, in case of M<N:
-0) If gene stated in A - place it under the A-n.
-1) If gene stated in D - above the D-n.
-2) If stated in both - deadbeef case, i've already said about it...

COLLISION CASES // TODO

*/
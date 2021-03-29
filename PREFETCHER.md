1. Describe how your prefetcher works.
   
   My prefetcher checks forward and backwards w/ the previous address to see if it is within the set/line in front or behind it. I checked 2* the amount as well to have a further reach.  It also does a count so that I may only prefetch when a certain strategy is actually being used. This allows for greater amount of control by saying you want to prefetch only when it occurs over X% of the time. It adapts so that if something is used and then stops being used, then it will eventually end that strategy. N in the command line is used for the X% and I found that 5-10% gave me the best hit ratio.

2. Explain how you chose that prefetch strategy.

   I saw several articles while googling around about adaptive strategy. I went with one originally that checked further ahead and behind to create a total, then I realized that I was making my code extra complicated and that it wasn't very hardware realistic. By checking only several places, it allowed me to not store too much information. I also checked sets ahead due to trace 3 and realized that I shouldn't just be checking the size of cache lines.

3. Discuss the pros and cons of your prefetch strategy.

   This strategy works well with proper spatial placement of memory, accessing stuff that is close, similar to the strength of adjacent and sequential. It works well when we access similar access over the entire time so that it may pick up on those patterns. Finding a good X% will allow the prefetcher to have a better hit ratio than adjacent and sequential almost all of the time when I was testing.  

4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).
   
   I read about several prefetchers that were adaptive in several different ways. My strategy would require very little processing as it is one subtraction and several compares. One thing that is a little much is that it requires 9 counters that could require a large amount of bits if lots of memory is being processed. The comparison of the last and current address I related to the hardware required to create the STRIDE algorithm. (prefetching with arbitrary strides). There are also several adaptive prefetchers already created which are more complicated and contain the hardware required for my strategy.  

5. Cite any additional sources that you used to develop your prefetcher.
   http://pages.cs.wisc.edu/~moravan/tap.pdf
   
   https://dl.acm.org/doi/abs/10.5555/1025127.1026003
   
   https://ieeexplore.ieee.org/abstract/document/7842926
/**
  \page configuration Configuration
  \section lists Lists

  \subsection sub_ring_downsize CONFIG_LEPTO_RING_DOWNSIZE
  Use different algorithm for data handling. This creates an spare entry in the 
  list which can not be filled. The usable size of list is "m_maxEntries-1".

  Saves 248 Bytes on canbridge

  \subsection sub_ring_no_threads CONFIG_LEPTO_RING_NO_THREADS
  Remove multi threading support on producer side for lists. Parallel feeding 
  in ISRs is still working. There it is assumed that consuming procedure is a 
  defined step in the event loop and 'half' produced entries are not possible.
  An ISR will be completely finished.
  
  Saves 128 Bytes on canbridge.


*/

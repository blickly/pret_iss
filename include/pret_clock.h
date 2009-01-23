/*
Copyright (c) 2007-2009 The Regents of the University of California.
All rights reserved.

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the
above copyright notice and the following two paragraphs appear in all
copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

PT_COPYRIGHT_VERSION_2
COPYRIGHTENDKEY
*/
#ifndef _PRET_CLOCK_H_
#define _PRET_CLOCK_H_

///////////////////////////////////////////////////////////////////////
/// pret_clock
/**
 * This class is a simple clock class that is used for both the main
 * clock  and the PLL clocks. It is only used when SystemC is disabled.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class pret_clock {
public:
    /** Default constructor sets a _period of 1 and initializes the _count
     *  to 0.
     */
    pret_clock();

    /** Set the _period to specified argument and _count to 0.
     *   @param period Period for the clock.
     */
    pret_clock(const unsigned int& period);
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
    /** Return the period of the clock instance.
    */
    unsigned int get_period() const;

    /** Return true if the clock is at a new period.
     *  @return True if a new period has started.
     */
    bool get_tick() const;

    /** Progress the clock by a single count/tick.
     */
    void tick();

///////////////////////////////////////////////////////////////////////
//                       private variables                          ///
private:
    /* Variable _count increments every tick. If it has incremented
     * _period counts more then a period is complete.
     */
    unsigned int _count;
    unsigned int _period;
};
#endif /* _PRET_CLOCK_H_ */

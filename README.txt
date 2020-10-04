HW #2

Dept. of Electrical and Computer Engineering
Donghoon Shin 2016-16578





1. Environment

OS: macOS Catalina (10.15.6, 19G2021))
CPP compiler: GNU++14

You can run Solitaire.cpp to get the result. The result should be
saved as "result.txt".





2. How to read the output

Below is a sample output that should be written on "result.txt".
As you may see, [Initial card sets] are shown first,
which is followed by [Game sequences] (movements) and
[End of game] (that also shows the point.)

The character 'R' that follows the number of a card
indicates whether the card is 'previously revealed (open)' or not.





Game 1

===========================

[Initial card sets]

Playing #1: ♡4R 
Playing #2: ♠9 ◇1R 
Playing #3: ◇2 ♠8 ◇9R 
Playing #4: ♡13 ♠13 ♣12 ♠10R 
Playing #5: ♡10 ♠6 ◇3 ♡1 ♡7R 
Playing #6: ♣10 ♣4 ♣13 ♠1 ♣6 ◇5R 
Playing #7: ♠12 ♡8 ♣3 ◇6 ◇7 ♡2 ♠4R 
Output #1: 
Output #2: 
Output #3: 
Output #4: 
Stock: ♠5 ◇11 ♡6 ♣8 ♣5 ♡12 ♠3 ♠7 ♡9 ♣1 ◇13 ◇10 ♡3 ♣9 ♡11 ♠2 ♣2 ◇8 ◇12 ♣11 ♣7 ♠11 ◇4 ♡5R 
Waste: 

===========================

[Game sequences]

◇1R from Playing #2 to Output #4
♡5R from Stock to Waste
◇9R from Playing #3 to Playing #4
♠8R from Playing #3 to Playing #4
◇2R from Playing #3 to Output #4
♡7R from Playing #5 to Playing #4
♡1R from Playing #5 to Output #3
◇3R from Playing #5 to Output #4
♡5R from Waste to Playing #5
◇4R from Stock to Waste
♡5R ♠6R from Playing #5 to Playing #4
◇4R from Waste to Output #4
◇5R from Playing #6 to Output #4
♠11R from Stock to Waste
♠9R from Playing #2 to Playing #5
♠4R from Playing #7 to Playing #4
♡2R from Playing #7 to Output #3
♣6R from Playing #6 to Playing #7
♠1R from Playing #6 to Output #1
♣13R from Playing #6 to Playing #2
♣7R from Stock to Waste
♣11R from Stock to Waste
◇12R from Stock to Waste
◇12R from Waste to Playing #2
♣11R from Waste to Playing #2
♠9R ♡10R from Playing #5 to Playing #2
◇8R from Stock to Waste
◇8R from Waste to Playing #2
♣7R from Waste to Playing #2
♣2R from Stock to Waste
♠2R from Stock to Waste
♠2R from Waste to Output #1
♡11R from Stock to Waste
♣9R from Stock to Waste
♡3R from Stock to Waste
♡3R from Waste to Output #3
♡4R from Playing #1 to Output #3
◇10R from Stock to Waste
◇13R from Stock to Waste
◇13R from Waste to Playing #1
♣1R from Stock to Waste
♣1R from Waste to Output #2
♡9R from Stock to Waste
♠7R from Stock to Waste
♠3R from Stock to Waste
♠3R from Waste to Output #1
♠4R from Playing #4 to Output #1
♡5R from Playing #4 to Output #3
♡12R from Stock to Waste
♣5R from Stock to Waste
♣8R from Stock to Waste
♡6R from Stock to Waste
♡6R from Waste to Output #3
◇11R from Stock to Waste
♠5R from Stock to Waste
♠5R from Waste to Output #1
♠6R from Playing #4 to Output #1
♡7R from Playing #4 to Output #3
♣6R ◇7R from Playing #7 to Playing #4
◇6R from Playing #7 to Output #4

===========================

[End of game]

Point: 48

===========================
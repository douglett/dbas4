function main()
	call room1()
end function

function room1()
	dim cmd
	print "you are in a room"
	print "the walls are made of stone and stuff."
	while 1
		print "what do you do?"
		input cmd
	end while
end function
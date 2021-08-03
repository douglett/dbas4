function main()
	call room1()
end function

function room1()
	dim string cmd
	print "you are in a room"
	print "the walls are made of stone and stuff."
	while 1
		print "what do you do?"
		input cmd
		print "you do: ", cmd
	end while
end function
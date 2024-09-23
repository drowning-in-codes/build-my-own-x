let frame = document.querySelector(".frame");
let p_1 = document.querySelector(".center p:nth-child(1)");
let p_2 = document.querySelector(".center p:nth-child(2)");
let p_3 = document.querySelector(".center p:nth-child(3)");

frame.addEventListener("click",(e)=>{
	if(p_1.classList.contains("move_down"))
	{
		p_1.classList.remove("move_down");
		p_1.classList.add("back_down");
	}else
		{
	p_1.classList.add("move_down");
		p_1.classList.remove("back_down");
		}
	
	
	if(p_3.classList.contains("move_up"))
	{
		p_3.classList.remove("move_up");
		p_3.classList.add("back_up");
	}else
		{
	p_3.classList.add("move_up");
		p_3.classList.remove("back_up");
		}
	
		if(p_2.classList.contains("vanish"))
	{
		p_2.classList.remove("vanish");
		p_2.classList.add("appear");
	}else
		{
	p_2.classList.add("vanish");
		p_2.classList.remove("appear");
		}
})
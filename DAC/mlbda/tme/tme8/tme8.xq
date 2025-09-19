(:~~~~~~~~~~~~~ex1~~~~~~~~~~~~~:)
(:Q1:)
(:for $p in //person
where $p/@id = "person1"
return $p/name:)

(:Q2:)
(:for $a in //open_auctions/auction[position() < 4]
return 
  <result id="{$a/@id}">
    {$a/initial}
  </result> :)

(:Q3:)
(:for $a in //open_auctions/auction[position() < 4]
let $first := $a/bidder[position() = 1]/increase/text()
let $last := $a/bidder[position() = last()]/increase/text()
return 
  <result id="{$a/@id}">
    <first>{$first}</first>
    <last>{$last}</last>
  </result>:)
  
(:Q4:) 
(:for $a in //closed_auctions/auction
let $i1 := $a/itemref/@item
let $i2 := //item[@id = $i1]
where $a/price > 480
return 
  <result>
    {$i2/name}
    {$a/price}
  </result>:) 


(:Q5:)
(:for $n in //africa/item/name
return
  <result>
    {$n}
  </result>:)
  
(:Q6:)
(:for $item in //africa/item
let $id := $item/@id
for $a in //closed_auctions/auction
where $a/itemref/@item = $id
return
  <result>
    {$item/name}
    {$a/price}
  </result>:)
  
(:Q7:)
let $person := //person[not(homepage)]
return count($person)




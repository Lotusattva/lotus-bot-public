# Cultivation Calculator Overview

**THIS DOCUMENT IS WORK IN PROGRESS**

This document outlines the calculation which would be performed by the bot to produce an estimate of how much longer it takes for a player to reach the next major cultivation stage.

This document will be divided into five sections, each focuses on a crucial source of cultivation exp income:
1. [Cultivation™ and Aura Gem](#cultivation-and-aura-gem)
2. [Respira](#respira)
3. [Cultivation Pills](#cultivation-pills)
4. [Myrimon Fruits and the Aura Extractor](#myrimon-fruits-and-the-aura-extractor)
5. [Creation Artifacts: the Vase and the Mirror](#creation-artifacts-the-vase-and-the-mirror)

## Cultivation™ and Aura Gem

Cultivation™ and aura gem are players' passive cultivation exp income sources, with the aura gem being dependent on cultivation speed, and thus are discussed as a whole in this section. 

The calculation is straight forward:

$$ ExpRate = AbodeAura \times (PlayerAbsorptionRate + AuraGemAbsorptionRate) $$

where:
- $ExpRate$ is the cultivation exp income per unit time
- $AuraGemAbsorptionRate$ is calculated as: $AuraGemAbsorptionRate = PlayerAbsorptionRate * AuraGemMultiplier$

Where $AuraGemMultiplier$ depends on the quality of the aura gem, and is defined in the following table:

| Aura Gem Quality | Aura Gem Multiplier |
|------------------|---------------------|
| Common           | 0.1                 |
| Uncommon         | 0.13                |
| Rare             | 0.16                |
| Epic             | 0.2                 |
| Legendary        | 0.24                |
| Mythic           | 0.28                |

## Respira

A player can perform a number of respira per day. Each respira attempt offers an amount of cultivation exp calculated as follows:

$$ Exp = RespiraBaseExp \times (1 + PlayerRespiraBonus)$$

where:
- $RespiraBaseExp$ is dependent on the player's major stage, as defined in the table below:
    <details><summary><ins>Click to show table</ins></summary>
    
    | Major Stage  | $RespiraBaseExp$ |
    |--------------|------------------|
    | Novice       | 6                |
    | Virtuoso     | 20               |
    | Foundation   | 110              |
    | Virtuoso     | 650              |
    | Nascent      | 3200             |
    | Incarnation  | 5300             |
    | Voidbreak    | 7800             |
    | Wholeness    | 10500            |
    | Perfection   | 13500            |
    | Nirvana      | 25000            |
    | Celestial    | 37500            |

    </details>

- $PlayerRespiraBonus$ is the player's sum of all respira bonuses they gain from various sources such as techniques and immortal friends.

Additionally, each respira attempt has a chance to be multiplied by a factor of 1, 2, 5, or 10, which chances defined in the table below:

| Respira Multiplier | Chance |
|--------------------|--------|
| 1                  | 55%    |
| 2                  | 30%    |
| 5                  | 14.75% |
| 10                 | 0.25%  |

Thus, each respira attempt has an expected value of:

$$ 
\begin{align*}
    E[\text{respira exp per attempt}] & = Exp \times (0.55 + 0.3 \cdot 2 + 0.1475 \cdot 5 + 0.0025 \cdot 10) \\
                                      & = Exp \times 1.9125 \\
                                      & = RespiraBaseExp \times (1 + PlayerRespiraBonus) \times 1.9125 \\
\end{align*}
$$

Where:
- $E[\text{respira exp per attempt}]$ is the expected cultivation exp gained from a single respira attempt

## Cultivation Pills

A player has a limited number of cultivation pill attempts per day. Each pill offers an amount of exp depending on its rank and quality and the player's cultivation pill bonuses. The calculation is as follows:
- For common to legendary pills:

    $$Exp = PillBaseExp \times (1 + PlayerPillBonus)$$

- For mythic pills:

    $$Exp = PillBaseExp \times (1 + PlayerPillBonus + VaseBonus)$$

Where:
- $Exp$ is the cultivation exp gained from consuming the pill

- $PillBaseExp$ is the base cultivation exp of the pill, which is dependent on the rank and quality of the pill. The base cultivation exp for each rank and quality is (partially) defined in the following table:
    <details><summary><ins>Click to show table</ins></summary>

    | Pill Rank | Common | Uncommon |  Rare  |  Epic  | Legendary | Mythic  |
    |-----------|--------|----------|--------|--------|-----------|---------|
    | 1         | 125    | 250      | 400    | 750    | 1500      | 3000    |
    | 2         | 625    | 1250     | 2000   | 3750   | 7500      | 15000   |
    | 3         | 1900   | 3800     | 6080   | 11400  | 22800     | 45600   |
    | 4         | 5000   | 10000    | 16000  | 30000  | 60000     | 120000  |
    | 5         | 8000   | 16000    | 25600  | 48000  | 96000     | 192000  |
    | 6         | 12000  | 24000    | 38400  | 72000  | 144000    | 288000  |
    | 7         | 20500  | 41000    | 65600  | 123000 | 246000    | 492000  |
    | 8         | 31000  | 62000    | 99200  | 186000 | 372000    | 744000  |
    | 9         | 57000  | 114000   | 182400 | 342000 | 684000    | 1368000 |
    | 10        | 128375 | 256750   | 410800 | 770250 | 1540500   | 3081000 |
    | 11        | *TODO* | *TODO*   | *TODO* | *TODO* | *TODO*    | *TODO*  |

    </details>

- $PlayerPillBonus$ is the player's sum of all cultivation pill bonuses they gain from various sources such as techniques, curios, and immortal friends. Unfortunately, the game does not display the player's total cultivation pill bonus, so the value has to be calculated. Doubly unfortunate, the game rounds numbers to 2 decimal places, so even if we attempt to calculate the player's total cultivation pill bonus, we may not be able to get the exact number.

- $VaseBonus$ is the bonus from the creation artifact vase, and it's only applied to mythic pills. This bonus is 10% if the vase is 1-2 stars, 20% if it is 3 stars and above. An additional 8% is added to the bonus if the player has the vase's transmog.


## Myrimon Fruits and the Aura Extractor

This section outlines how cultivation exp from consuming myrimom fruits is effected by 3 nodes of the aura extractor: CultiXP, Quality, and Gush. Each of these nodes introduces multiplicative bonuses that are applied to the base exp of a myrimon fruit with different conditions.

### CultiXP

The "CultiXP" node introduces a multiplicative bonus with two additive components: one that depends on the node's level, and one that depends on the node's quality.

1. The first component is calculated based on the level of the "CultiXP" node as follows:

    $$CultiXPLevelMult = CultiXPNodeLevel \times CultiXPPerLevelMult$$
    
    Where:
    - $CultiXPLevelMult$ is the multiplicative bonus applied to the exp of the resulting orb based on the level of the "CultiXP" node
    - $CultiXPNodeLevel$ is the level of the "CultiXP" node
    - $CultiXPPerLevelMult$ is the multiplier per node level depending on the "world" level corresponding to the player's major stage, as defined in the following table:
        | World    | $CultiXPPerLevelMult$ |
        |----------|-----------------------|
        | Mortal   | 0.02                  |
        | Spirit   | 0.04                  |
        | Immortal | *Currently no data*   |

2. The second component is a 20% bonus conditional whether the quality of the "CultiXP" node is at or above the quality of the exp orb produced by the aura extractor upon consuming a myrimon fruit (e.g. if the orb is RARE, and the "CultiXP" extractor node is at RARE or higher, then the 20% bonus applies). 

    The expectancy of this bonus is calculated as follows:

    $$CultiXPQualityMult = 0.2 \cdot \Pr(\text{CultiXP quality >= orb quality})$$

    The probability distribution of the orb quality is explored in the next section.

Taken together, the expected multiplicative bonus applied to the exp of the resulting orb based on the "CultiXP" node is calculated as follows:

$$
CultiXPMult = 1 + CultiXPLevelMult + CulitiXPQualityMult
$$

### Quality
Another multiplicative bonus is applied to the exp of the resulting orb based on the quality of that orb, as defined in the following table:
| Orb Quality | Multiplier |
|-------------|------------|
| Common      | 1.0        |
| Uncommon    | 1.3        |
| Rare        | 1.6        |
| Epic        | 2.0        |
| Legendary   | 2.4        |
| Mythic      | 3.0        |

The quality of the exp orb produced by the aura extractor upon consuming a myrimon fruit is dependent on 1) the quality of the aura extractor itself, and 2) the level of the "Quality" node:
    
1. The aura extractor adds a 30% chance to produce an orb of the same quality as itself, e.g., if the extractor is of *Epic* quality, then an orb of *Epic* quality would have a 30% higher chance to be produced.

2. The level of the "Quality" node distributes the remaining 70% chance among qualities as defined in the following table:
    
    <details><summary><ins>Click to show table</ins></summary>

    | Node Level | Common | Uncommon | Rare  | Epic  | Legendary | Mythic |
    |------------|--------|----------|-------|-------|-----------|--------|
    | 0          | 70%    | 0%       | 0%    | 0%    | 0%        | 0%     |
    | 1          | 65%    | 5%       | 0%    | 0%    | 0%        | 0%     |
    | 2          | 60%    | 10%      | 0%    | 0%    | 0%        | 0%     |
    | 3          | 55%    | 15%      | 0%    | 0%    | 0%        | 0%     |
    | 4          | 50%    | 25%      | 0%    | 0%    | 0%        | 0%     |
    | 5          | 45%    | 25%      | 0%    | 0%    | 0%        | 0%     |
    | 6          | 20%    | 50%      | 0%    | 0%    | 0%        | 0%     |
    | 7          | 15%    | 55%      | 0%    | 0%    | 0%        | 0%     |
    | 8          | 10%    | 60%      | 0%    | 0%    | 0%        | 0%     |
    | 9          | 5%     | 65%      | 0%    | 0%    | 0%        | 0%     |
    | 10         | 0%     | 70%      | 0%    | 0%    | 0%        | 0%     |
    | 11         | 0%     | 40%      | 30%   | 0%    | 0%        | 0%     |
    | 12         | 0%     | 30%      | 40%   | 0%    | 0%        | 0%     |
    | 13         | 0%     | 20%      | 50%   | 0%    | 0%        | 0%     |
    | 14         | 0%     | 10%      | 60%   | 0%    | 0%        | 0%     |
    | 15         | 0%     | 0%       | 70%   | 0%    | 0%        | 0%     |
    | 16         | 0%     | 0%       | 40%   | 30%   | 0%        | 0%     |
    | 17         | 0%     | 0%       | 30%   | 40%   | 0%        | 0%     |
    | 18         | 0%     | 0%       | 20%   | 50%   | 0%        | 0%     |
    | 19         | 0%     | 0%       | 10%   | 60%   | 0%        | 0%     |
    | 20         | 0%     | 0%       | 0%    | 70%   | 0%        | 0%     |
    | 21         | 0%     | 0%       | 0%    | 40%   | 30%       | 0%     |
    | 22         | 0%     | 0%       | 0%    | 30%   | 40%       | 0%     |
    | 23         | 0%     | 0%       | 0%    | 20%   | 50%       | 0%     |
    | 24         | 0%     | 0%       | 0%    | 10%   | 60%       | 0%     |
    | 25         | 0%     | 0%       | 0%    | 0%    | 70%       | 0%     |
    | 26         | 0%     | 0%       | 0%    | 0%    | 40%       | 30%    |
    | 27         | 0%     | 0%       | 0%    | 0%    | 30%       | 40%    |
    | 28         | 0%     | 0%       | 0%    | 0%    | 20%       | 50%    |
    | 29         | 0%     | 0%       | 0%    | 0%    | 10%       | 60%    |
    | 30         | 0%     | 0%       | 0%    | 0%    | 0%        | 70%    |

    </details>

> For example, if the aura extractor is of *Epic* quality and the "Quality" node is at level 16, then the player has a 40% chance of getting a *Rare* orb and a 60% chance of getting an *Epic* orb. 
> - 40% chance of getting a *Rare* orb comes from "Quality" node at level 16
> - 60% chance of getting an *Epic* orb comes from two sources:
>   - 30% chance of getting an *Epic* orb from the aura extractor itself being of *Epic* quality
>   - 30% chance of getting an *Epic* orb from the "Quality" node at level 16

Then, the expected quality multiplier can be calculated as follows:

$$
E[\text{quality multiplier}] = \sum^{i}[QualityChance_i \times QualityMult_i]
$$

Where $i$ is quality from common to mythic.

### Gush
Consuming a myrimon fruit has a chance to gush. When a gush occurs, a multiplicative bonus based on the level of the "Gush" extractor node is applied to the exp of the resulting orb. This bonus is dependent on the level of the "Gush" node, which is calculated as follows:

$$ GushMult = BaseGushMult + GushNodeLevel \cdot GushMultPerNodeLevel $$

Where:
- $GushMult$ is the multiplicative bonus applied to the exp of the resulting orb when a gush occurs
- $BaseGushMult$ is the base gush multiplier, which is 1.50
- $GushNodeLevel$ is the level of the "Gush" extractor node
- $GushMultPerNodeLevel$ is the gush multiplier per node level, which is 0.04

For reference, when the "Gush" node is maxed (level 30), the gush multiplier is 2.70.

The chance to gush is somewhat complicated. There is also a pity system such that a gush is guaranteed to trigger on the 6th fruit if consuming the first 5 fruits did not gush. Collecting information about the player's current position in the pity cycle and doing calculations from there is cumbersome for both the bot and the player. Therefore, to simplify this process, we will use a consolidated per-use gush chance.

To derive the consolidated per-use gush chance, we first consider the probability of getting a gush before the 6th fruit. Let $k$ be the number of fruits consumed, and $k \in [1, 5], k \in \mathbb{Z}$. Let $P$ be the chance to gush. The probability of getting a gush before the 6th fruit is given by:

$$\Pr(\text{trigger on } k) = (1 - P)^{k-1} \cdot P$$

Where:
- $(1 - P)^{k-1}$ is the probability of not getting a gush in the first $k-1$ fruits
- $P$ is the probability of getting a gush on the $k\text{th}$ fruit

The probability of a pity trigger is given by:

$$\Pr(\text{pity}) = \Pr(\text{trigger on }6) = (1 - P)^5 \cdot 100 \\% = (1 - P)^5$$

Where:
- $(1 - P)^5$ is the probability of not getting a gush in the first 5 fruits
- $100\\%$ is the probability of getting a gush on the 6th fruit, i.e., the pity

Then, we can compute the expected number of fruits consumed per gush as follows:

$$E[\text{fruits per pity cycle}] = \sum_{k=1}^{5} k \cdot \Pr(\text{trigger on } k) + 6 \cdot \Pr(\text{pity})$$

Finally, let:
- $\text{Numerator} = 1$ be the number of gushes per pity cycle
- $\text{Denominator} = E[\text{fruits per pity cycle}]$ be the expected number of fruits consumed in each pity cycle

We can compute the consolidated per-use gush chance as follows:

$$
\begin{align*}
    P_{\text{consolidated}} & = \frac{\text{Numerator}}{\text{Denominator}} \\
                            & = \frac{1}{\sum_{k=1}^{5} k \cdot \Pr(\text{trigger on } k) + 6 \cdot \Pr(\text{pity})} \\
                            & = \frac{1}{\sum_{k=1}^{5} k \cdot (1 - P)^{k-1} \cdot P + 6 \cdot (1 - P)^5} \\
                            & = \frac{1}{(2-P)\cdot(3+(P-3) \cdot P)\cdot(1+(P-1) \cdot P)} \\
\end{align*}
$$

as a function of $P$.

The chance to gush $P$ is dependent on the quality of the "Quality" aura extractor node, which is defined in the following table:

| Node Quality | $P$  |
|--------------|------|
| Common       | 0.1  |
| Uncommon     | 0.15 |
| Rare         | 0.2  |
| Epic         | 0.25 |
| Legendary    | 0.3  |
| Mythic       | 0.35 |

Then, we can calculate the expected gush multiplier from the consolidated chance:

$$
E[\text{gush multiplier}] = GushMult \times P_{\text{consolidated}}
$$

### Conclusion
Taken together, the expected exp the player gets from consuming a myrimom fruit is calculated as follows:


## Creation Artifacts: the Vase and the Mirror

## Vase
| Vase Rank | Gain/Taoist year| Cap | Energy per Pill, Epic, Legendary | Pills a day / Pills a day with Epic, Legendary/ Pills a day with charge/ Pills a day with charge, Epic, Legendary |
|-----------|-----------------|-----|----------------------------------|-------------------------------------------------------------------------------------------------------------------|
| 1         | +1              | 300 | 100 / 90                         | 0.96 / 1.06 / 1.96 / 2.06                                                                                         |
| 2         | +1.5            | 400 | 100 / 90                         | 1.44 / 1.56 / 2.44 / 2.56                                                                                         |
| 3         | +2              | 500 | 100 / 90                         | 1.92 / 2.02 / 2.92 / 3.02                                                                                         |
| 4         | +2.5            | 600 | 100 / 90                         | 2.40 / 2.50 / 3.40 / 3.50                                                                                         |
| 5         | +3              | 700 | 100 / 90                         | 2.88 / 2.98 / 3.88 / 3.98                                                                                         |

Get 100 Energy a day with charge
  
- Rank 1 10% increase to exp for crafted pills
- Rank 3 exp for crafted pills increased to 20% instead of 10%
- Rank 5 15% chance to double craft
- Jade Purity Vase Transmog +8% increased exp for crafted pills


## Mirror
*Based on ingame data will update tomorrow to real figures*
| Mirror Rank | Gain/Taoist year| Cap | Mythic duplication Gain a day, Gain with charge |
|-------------|-----------------|-----|-------------------------------------------------|
| 1           | +1              | 300 | 0.48  / 0.98                                    |
| 2           | +1.6            | 400 | 0.768 / 1.268                                   |
| 3           | +2              | 500 | 0.96  / 1.46                                    |
| 4           | +2.5            | 600 | 1.20  / 1.70                                    |
| 5           | +3              | 700 | 1.44  / 1.94                                    |


Get 100 Energy a day with charge
Mythic Duplication Cost 190 for R7

- Rank 1 -5% Energy cost for Duplication
- Rank 3 Energy Reduction cost increased to 10%
- Rank 5 15% chance to obtain a Duplicate


Todo!

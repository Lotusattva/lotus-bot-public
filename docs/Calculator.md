# Cultivation Calculator Overview

This document outlines the calculation which would be performed by the bot to produce an estimate of how much longer it takes for a player to reach the next major cultivation stage.

This document will be divided into four sections, each focuses on a crucial source of cultivation exp income:
1. [Cultivation™ and Aura Gem](#cultivation-and-aura-gem)
2. [Cultivation Pills](#cultivation-pills)
3. [Myrimon Fruits and the Aura Extractor](#myrimon-fruits-and-the-aura-extractor)
4. [Creation Aftifacts: the Vase and the Mirror](#creation-artifacts-the-vase-and-the-mirror)

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

## Cultivation Pills

A player has a limited number of cultivation pill attempts per day. Each pill offers an amount of exp depending on its rank and quality and the player's cultivation pill bonuses. The calculation is as follows:
- For common to legendary pills:\
$$Exp = PillBaseExp \times (1 + PlayerPillBonus)$$
- For mythic pills:\
$$Exp = PillBaseExp \times (1 + PlayerPillBonus + VaseBonus)$$

Where:
- $Exp$ is the cultivation exp gained from consuming the pill

- $PillBaseExp$ is the base cultivation exp of the pill, which is dependent on the rank and quality of the pill. The base cultivation exp for each rank and quality is (partially) defined in the following table:

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
    | 12        | *TODO* | *TODO*   | *TODO* | *TODO* | *TODO*    | *TODO*  |

- $PlayerPillBonus$ is the player's sum of all cultivation pill bonuses they gain from various sources such as techniques, curios, and immortal friends. Unfortunately, the game does not display the player's total cultivation pill bonus, so the value has to be calculated. Doubly unfortunate, the game rounds numbers to 2 decimal places, so even if we attempt to calculate the player's total cultivation pill bonus, we may not be able to get the exact number.

- $VaseBonus$ is the bonus from the creation artifact vase, and it's only applied to mythic pills. This bonus is 10% if the vase is 1-2 stars, 20% if it is 3 stars and above. An additional 8% is added to the bonus if the player has the vase's transmog.


## Myrimon Fruits and the Aura Extractor

### Gush chance

Consuming a myrimon fruit has a chance to gush, and there is also a pity system such that a gush is guaranteed to trigger on the 6th fruit if consuming the first 5 fruits did not gush. Collecting information about the player's current position in the pity cycle and doing calculations from there is cumbersome for both the bot and the player. Therefore, to simplify this process, we will use a consolidated per-use gush chance.

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

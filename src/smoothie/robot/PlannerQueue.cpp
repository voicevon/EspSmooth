// #include "PlannerQueue.h"
// #include "esp_attr.h"

// IRAM_ATTR Block* PlannerQueue::get_tail() {
//     if (empty())
//         return nullptr;
//     return &m_buffer[m_rIndex];
// }

// IRAM_ATTR void PlannerQueue::release_tail(void) {
//     if(empty()) return; // this should not happen as we should never call this if we did not get a valid tail
//     m_rIndex = next(m_rIndex);
// }

// IRAM_ATTR bool PlannerQueue::empty() const {
//     return (m_rIndex == m_wIndex);
// }